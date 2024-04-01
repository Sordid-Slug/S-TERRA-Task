CC = gcc
CFLAGS = -c -Wall -Werror -Wextra

all: main

main: main.o dblLinkedList.o
	$(CC) main.o dblLinkedList.o -o main -lpthread
	rm -rf *.o

main.o: main.c
	$(CC) $(CFLAGS) main.c -lpthread

dblLinkedList.o: dblLinkedList.c
	$(CC) $(CFLAGS) dblLinkedList.c

rebuild: clean all

clean:
	rm -rf main thread main2 *.o