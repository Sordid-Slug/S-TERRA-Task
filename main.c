#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "dblLinkedList.h"

typedef struct _Params {
    dblLinkedList *list;
    int bit;
    void*(*deleteNodeFunc)(dblLinkedList *);
} Params;

typedef struct _Returns {
    int processed;
    int count;
} Returns;

int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int count_bits_of_value(int value, int bit) {
    count += 1;
    int count = 0;
    while (value > 0) {
            if ((value & 1) == bit) {
                count += 1;
            }
            value = value >> 1;
        }
    return count;
}

void *bits_count(void *params) {
    Params *param_args = (Params *)params;
    Returns *return_values = (Returns *)calloc(1, sizeof(Returns));
    if (return_values == NULL) {
        printf("Память не выделилась корректно\n");
    }
    dblLinkedList *list = param_args->list;
    
    while (1) {
        if (pthread_mutex_lock(&mutex)) {
            printf("мьютекс не заблокировался");
            exit(7);
        }
        if (list->head == 0x0 || list->tail == 0x0) {   // Я не имею ни малейшего понятия, почему, но
            pthread_mutex_unlock(&mutex);               // условие while (list->head != 0x0 && list->tail != 0x0) не всегда работает,
            break;                                      // даже когда list->head и list->tail равны 0x0
        }
        int *value = (int *)param_args->deleteNodeFunc(list);
        return_values->count += count_bits_of_value(*value, param_args->bit);
        pthread_mutex_unlock(&mutex);
        return_values->processed += 1;
        free(value);
    }
    pthread_exit((void *)return_values);
}

int main(int argc, char **argv) {
    int size;
    if (argc != 2) {
		printf("Может быть один аргумент - размер списка\n");
        exit(1);
    } else if ((size = atoi(argv[1])) == 0 && (*argv)[1] != '0') {
        printf("Введите только одно число, а не буквы\n");
        exit(2);
    } else if (size < 0) {
        printf("Введённый размер списка должен быть неотрицательным\n");
        exit(8);
    }
    dblLinkedList list;
    createList(&list);
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		int value = rand();
		pushback(&list, &value, sizeof(int));
	}
    pthread_t pid;
    pthread_t pid2;

    Params paramsForZeroes = {&list, 0, popFront};
    Params paramsForOnes = {&list, 1, popBack};

    if (pthread_create(&pid, NULL, bits_count, &paramsForZeroes) != 0) {
        perror("Поток не создался корректно");
        exit(5);
    }
    if (pthread_create(&pid2, NULL, bits_count, &paramsForOnes) != 0) {
        perror("Поток не создался корректно");
        exit(6);
    }
    Returns* zeroes;
    Returns* ones;

	pthread_join(pid, (void*)&zeroes);
	pthread_join(pid2, (void*)&ones);

    printf("\nZeroes:\ncount: %d\nprocessed: %d\n\nOnes:\ncount: %d\nprocessed: %d\n%d\n",
     zeroes->count, zeroes->processed, ones->count, ones->processed, count);

    free(zeroes);
    free(ones);
    return 0;
}
