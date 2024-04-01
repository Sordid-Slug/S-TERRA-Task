#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "dblLinkedList.h"

typedef struct _Params {
    dblLinkedList *list;
    int bit;
} Params;

typedef struct _Returns {
    int processed;
    int count;
} Returns;

int count_bits(int value, int bit) {
    int count = 0;
    while (value > 0) {
            if (value & 1 == bit) {
                count += 1;
            }
            value = value >> 1;
        }
    return 0;
}

void *one_bits_count(void *params) {
    Params *param_args = (Params *)params;
    Returns *return_values = (Returns *)calloc(1, sizeof(Returns));
    dblLinkedList *list = param_args->list;

    int i = 0;
    while (list->tail != NULL && list->size > 0) {
        int *value = (int *)list->tail->value;
        return_values->count += count_bits(*value, param_args->bit);
        return_values->processed += 1;

        // printf("единицы [%d] %ld\n", i, list->size);
        if (popBack(list) == 1) {
            printf("Удалять нечего для нулей\n");
            break;
        }
        printf("единицы [%d] %ld\n", i, list->size);
        i++;
    }
    pthread_exit((void *)return_values);
}

void *zero_bits_count(void *params) {
    Params *param_args = (Params *)params;
    Returns *return_values = (Returns *)calloc(1, sizeof(Returns));
    dblLinkedList *list = param_args->list;

    int i = 0;
    while (list->head != NULL && list->size > 0) {
        int *value = (int *)list->head->value;
        while ((*value) > 0) {
            if ((*value & 1) == param_args->bit) {
                return_values->count += 1;
            }
            *value = *value >> 1;
        }
        return_values->processed += 1;
        // printf("{%d} ", return_values->count);
        if (popFront(list) == 1) {
            printf("Удалять нечего для нулей\n");
            break;
        }
        printf("нули     [%d] %ld\n", i, list->size);
        i++;
    }
    pthread_exit((void *)return_values);
}

int main(int argc, char **argv) {
    int size;
    if (argc > 5) {
		printf("Может быть один аргумент - размер списка\n");
        exit(1);
    } else if ((size = atoi(argv[1])) == 0 && (*argv)[1] != '0') {
        printf("Введите только одно число\n");
        exit(2);
    }
    dblLinkedList* list = createList();
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		int value = rand();
		pushback(&list, &value, sizeof(int));
	}

    pthread_t pid;
    pthread_t pid2;

    Params params = {list, 0};

    if (pthread_create(&pid, NULL, zero_bits_count, &params) != 0) {
        printf("Thread didn't create correctly");
        exit(5);
    }
    params.bit = 1;
    if (pthread_create(&pid2, NULL, one_bits_count, &params) != 0) {
        printf("Thread didn't create correctly");
        exit(6);
    }
    void *res_first_thread;
    void *res_second_thread;
	pthread_join(pid, &res_first_thread);
	pthread_join(pid2, &res_second_thread);
    Returns* zeroes = (Returns *)res_first_thread;
    Returns* ones = (Returns *)res_second_thread;

    printf("\nZeroes:\ncount: %d\nprocessed: %d\n\nOnes:\ncount: %d\nprocessed: %d\n", zeroes->count, zeroes->processed, ones->count, ones->processed);
    free(res_first_thread);
    free(res_second_thread);
    free(list);
    return 0;
}