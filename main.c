#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "dblLinkedList.h"

dblLinkedList *createList() {
	dblLinkedList* tmp = (dblLinkedList*)calloc(1, sizeof(dblLinkedList));
	if (!tmp) {
		exit(1);
	}
	tmp->size = 0;
	tmp->head = tmp->tail = NULL;

	return tmp;
}

void deleteList(dblLinkedList** list) {
	Node* tmp = (*list)->head;
	Node* next = NULL;
	while (tmp) {
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	free(*list);
	*list = NULL;
}

void pushback(dblLinkedList** list, void* value, size_t value_size) {
	Node* tmp = (Node*)calloc(1, sizeof(Node));
	if (tmp == NULL) {
		exit(2);
	}

	tmp->value = calloc(1, value_size);

	memcpy(tmp->value, value, value_size);
	tmp->prev = (*list)->tail;
	tmp->next = NULL;
	if ((*list)->tail != NULL) {
		(*list)->tail->next = tmp;
	}
	(*list)->tail = tmp;
	if ((*list)->head == NULL) {
		(*list)->head = tmp;
	}
	(*list)->size++;
}

void printList(dblLinkedList *list, void(*print_func)(void*)) {
	Node* tmp = list->head;
	while (tmp) {
		print_func(tmp->value);
		tmp = tmp->next;
	}
	printf("\n");
}

void printInt(void* value) {
	printf("%d ", *((int*)value));
}

void printDouble(void* value) {
	printf("%lf ", *((double*)value));
}

int main() {
	dblLinkedList* list = createList();
	int size = 0;
	if (scanf("%d", &size) != 1) {
		printf("Incorrect input");
	}
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		int value = rand();
		pushback(&list, &value, sizeof(int));
	}
	printList(list, printInt);
	deleteList(&list);
	return 0;
}