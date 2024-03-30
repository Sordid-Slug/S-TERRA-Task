#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "dblLinkedList.h"

typedef struct _Params {
    dblLinkedList *list;
    int bit;
} Params;

typedef struct _Returns {
    int processed;
    int count;
} Returns;

Returns* zero_bits_count(void* params) {
    Params* param_args = (Params*)params;
    Returns* return_values = (Returns*)calloc(1, sizeof(Returns));
    dblLinkedList* list = param_args->list;

    Node* current = list->head;
    while (current && list->size > 0) {
        int* value = (int *)delNode(list, list->size - 1);
        while ((*value) > 0) {
            if ((*value & 1) == param_args->bit) {
                return_values->count += 1;
            }
            *value = *value >> 1;
        }
        return_values->processed += 1;

        printf("{%d} ", return_values->count);
    }
    return return_values;
    // pthread_exit((void *)return_values);
}

int main(int argc, char *argv[]) {
    int a;
    if (argc != 2) {
        exit(1);
    } else if ((a = atoi(argv[1])) == 0 && (*argv)[1] != '0') {
        printf("Введите только одно число\n");
        exit(2);
    }
   int size = atoi(argv[1]);
    dblLinkedList* list = createList();
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        int value = rand();
        pushback(&list, &value, sizeof(int));
    }
    printList(list, printInt);
    // printf("_%d_\n", *(int*)(delNode(list, 3)));

    // pthread_t pid;
    // pthread_attr_t attr;
    // pthread_attr_init(&attr);

    Params params = { list, 1 };
    // pthread_create(&pid, &attr, zero_bits_count, &params);
    Returns* first_thread = zero_bits_count(&params);
    // pthread_join(pid, &first_thread);
    free(first_thread);
    deleteList(&list);
    return 0;

    return 0;

    
}