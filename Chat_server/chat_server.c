#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/stat.h>

#define PORT 12345
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct {
    int fd;
    char name[32];
} Client;

Client clients[MAX_CLIENTS];
size_t client_count = 0;
int server_fd;
FILE *log_file;

void log_activity(const char *message) {
    fprintf(log_file, "%s\n", message);
    fflush(log_file);
}

void broadcast_message(const char *message, int exclude_fd) {
    for (size_t i = 0; i < client_count; i++) {
        if (clients[i].fd != exclude_fd) {
            send(clients[i].fd, message, strlen(message), 0);
        }
    }
}

void remove_client(int fd) {
    for (size_t i = 0; i < client_count; i++) {
        if (clients[i].fd == fd) {
            close(clients[i].fd);
            clients[i] = clients[client_count - 1];
            client_count--;
            return;
        }
    }
}

void handle_client_message(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) {
        remove_client(client_fd);
        log_activity("Client disconnected.");
        return;
    }

    buffer[bytes_read] = '\0';
    if (strncmp(buffer, "/name ", 6) == 0) {
        for (size_t i = 0; i < client_count; i++) {
            if (clients[i].fd == client_fd) {
                strncpy(clients[i].name, buffer + 6, 31);
                clients[i].name[31] = '\0';
                char message[BUFFER_SIZE];
                snprintf(message, BUFFER_SIZE, "User renamed to %s\n", clients[i].name);
                log_activity(message);
                return;
            }
        }
    } else if (strncmp(buffer, "/quit", 5) == 0) {
        remove_client(client_fd);
        log_activity("Client disconnected.");
    } else {
        char message[BUFFER_SIZE + 64];
        snprintf(message, sizeof(message), "%s: %s", "Anonymous", buffer);
        for (size_t i = 0; i < client_count; i++) {
            if (clients[i].fd == client_fd) {
                snprintf(message, sizeof(message), "%s: %s", clients[i].name, buffer);
                break;
            }
        }
        broadcast_message(message, client_fd);
        log_activity(message);
    }
}

void handle_signal(int signo) {
    if (signo == SIGINT || signo == SIGTERM) {
        log_activity("Server shutting down.");
        fclose(log_file);
        close(server_fd);
        exit(0);
    }
}

int main() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    struct pollfd poll_fds[MAX_CLIENTS + 1];
    char buffer[BUFFER_SIZE];

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    log_file = fopen("chat_server.log", "a");
    if (!log_file) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    log_activity("Server started.");

    poll_fds[0].fd = server_fd;
    poll_fds[0].events = POLLIN;

    while (1) {
        int poll_count = poll(poll_fds, client_count + 1, -1);
        if (poll_count < 0) {
            perror("Poll failed");
            break;
        }

        if (poll_fds[0].revents & POLLIN) {
            int new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
            if (new_fd < 0) {
                perror("Accept failed");
                continue;
            }

            if (client_count < MAX_CLIENTS) {
                clients[client_count].fd = new_fd;
                strcpy(clients[client_count].name, "Anonymous");
                client_count++;

                poll_fds[client_count].fd = new_fd;
                poll_fds[client_count].events = POLLIN;

                snprintf(buffer, BUFFER_SIZE, "New connection from %s:%d\n",
                         inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                log_activity(buffer);
            } else {
                close(new_fd);
            }
        }

        for (size_t i = 1; i <= client_count; i++) {
            if (poll_fds[i].revents & POLLIN) {
                handle_client_message(poll_fds[i].fd);
            }
        }
    }

    fclose(log_file);
    close(server_fd);
    return 0;
}
