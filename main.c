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

void *one_bits_count(void *params) {
    Params *param_args = (Params *)params;
    Returns *return_values = (Returns *)calloc(1, sizeof(Returns));
    dblLinkedList *list = param_args->list;
    
    Node *current = list->tail;
    while (current && list->size > 0) {
        int *value = (int *)popBack(list);
        while ((*value) > 0) {
            if ((*value & 1) == param_args->bit) {
                return_values->count += 1;
            }
            *value = *value >> 1;
        }
        return_values->processed += 1;
    }
    pthread_exit((void *)return_values);
}

void *zero_bits_count(void *params) {
    Params *param_args = (Params *)params;
    Returns *return_values = (Returns *)calloc(1, sizeof(Returns));
    dblLinkedList *list = param_args->list;
    
    Node *current = list->tail;
    while (current && list->size > 0) {
        int *value = (int *)popFront(list);
        while ((*value) > 0) {
            if ((*value & 1) == param_args->bit) {
                return_values->count += 1;
            }
            *value = *value >> 1;
        }
        return_values->processed += 1;

        // printf("{%d} ", return_values->count);
    }
    pthread_exit((void *)return_values);
}

int main(int argc, char **argv) {
    int size;
    if (argc != 2) {
		printf("Всего может быть один аргумент - размер списка\n");
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
	// printList(list, printInt);

    pthread_t pid, pid2;

    Params params = {list, 0};
    if (pthread_create(&pid, NULL, one_bits_count, &params) != 0) {
        printf("Thread didn't create correctly");
        exit(5);
    }
    params.bit = 1;
    if (pthread_create(&pid2, NULL, one_bits_count, &params) != 0) {
        printf("Thread didn't create correctly");
        exit(6);
    }
    void *res_first_thread, *res_second_thread;
	pthread_join(pid, &res_first_thread);
	pthread_join(pid2, &res_second_thread);
    Returns* zeroes = (Returns *)res_first_thread;
    Returns* ones = (Returns *)res_second_thread;

    printf("%d %d", zeroes->count, ones->count);
    free(res_first_thread);
    free(res_second_thread);
    deleteList(&list);
    return 0;
}