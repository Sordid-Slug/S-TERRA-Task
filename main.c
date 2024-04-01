#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "dblLinkedList.h"

typedef struct _Params {
    dblLinkedList *list;
    int bit;
    void(*deleteNodeFunc)(dblLinkedList *);
    Node *start;
} Params;

typedef struct _Returns {
    int processed;
    int count;
} Returns;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int count_bits_of_value(int value, int bit) {
    int count = 0;
    while (value > 0) {
            if ((value & 1) == bit) {
                count += 1;
            }
            value = value >> 1;
        }
    return count;
}

// void *one_bits_count(void *params) {
//     pthread_mutex_lock(&mutex);
//     Params *param_args = (Params *)params;
//     Returns *return_values = (Returns *)calloc(1, sizeof(Returns));
//     dblLinkedList *list = param_args->list;
//     int i = 0;
//     while (list->tail != NULL && list->size > 0) {
//         printf("единицы [%d] %ld\n", i, list->size);
//         int *value = (int *)list->tail->value;
//         return_values->count += count_bits(*value, param_args->bit);
//         return_values->processed += 1;
//
//         param_args->deleteNodeFunc(list);
//         i++;
//     }
//     pthread_mutex_unlock(&mutex);
//     pthread_exit((void *)return_values);
// }

void *bits_count(void *params) {
    pthread_mutex_lock(&mutex);
    Params *param_args = (Params *)params;
    Returns *return_values = (Returns *)calloc(1, sizeof(Returns));
    dblLinkedList *list = param_args->list;
    
    int i = 0;
    while (param_args->start != NULL && list->size > 0) {
        if (param_args->bit == 0) {
            param_args->start = list->head;
        } else {
            param_args->start = list->tail;
        }
        int *value = (int *)param_args->start->value;
        return_values->count += count_bits_of_value(*value, param_args->bit);
        return_values->processed += 1;

        param_args->deleteNodeFunc(list);
        i++;
    }
    pthread_mutex_unlock(&mutex);
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
    // Делим список на две половины
    dblLinkedList* listForZeroes = createList();
    dblLinkedList* listForOnes = createList();

    listForZeroes->head = list->head;
    listForZeroes->tail = getNode(list, list->size / 2);
    listForZeroes->size = list->size / 2;

    listForOnes->head = getNode(list, list->size / 2 + 1);
    listForOnes->tail = list->tail;
    listForOnes->size = list->size % 2 > 0 ? list->size / 2 + 1 : list->size / 2;

    Params paramsForZeroes = {listForZeroes, 0, popFront, listForZeroes->head};
    Params paramsForOnes = {listForOnes, 1, popBack, listForOnes->tail};

    if (pthread_create(&pid, NULL, bits_count, &paramsForZeroes) != 0) {
        printf("Thread didn't create correctly");
        exit(5);
    }
    if (pthread_create(&pid2, NULL, bits_count, &paramsForOnes) != 0) {
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
    free(listForOnes);
    free(listForZeroes);
    return 0;
}