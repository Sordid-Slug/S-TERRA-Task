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

Returns* ones_bits_count(void* params) {
    Params* param_args = (Params*)params;
    Returns* return_values = (Returns*)calloc(1, sizeof(Returns));
    dblLinkedList* list = param_args->list;

    Node *current = list->head;
    while (current && list->size > 0) {
        int *value = (int *)current->value;
        while ((*value) > 0) {
            if ((*value & 1) == param_args->bit) {
                return_values->count += 1;
            }
            *value = *value >> 1;
        }
        return_values->processed += 1;

        popFront(list);
        current = list->head;
    }
    return return_values;
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

    // Params params = { list, 1 };
    // Returns* first_thread = ones_bits_count(&params);
    // free(first_thread);
    while (list->size > 0) {
        popFront(list);
    }
    return 0;    
}