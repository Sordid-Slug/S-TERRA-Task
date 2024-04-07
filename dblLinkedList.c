#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dblLinkedList.h"

dblLinkedList* createList() {
	dblLinkedList* tmp = (dblLinkedList*)calloc(1, sizeof(dblLinkedList));
	if (!tmp) {
		perror("Память не выделилась корректно\n");
		exit(1);
	}
	tmp->head = tmp->tail = NULL;

	return tmp;
}

void deleteList(dblLinkedList** list) {
	Node* tmp = (*list)->head;
	Node* next = NULL;
	while (tmp) {
		next = tmp->next;
		free(tmp->value);
		free(tmp);
		tmp = next;
	}
	free(*list);
	*list = NULL;
}

void pushback(dblLinkedList** list, void* value, size_t value_size) {
	Node* tmp = (Node*)calloc(1, sizeof(Node));
	if (tmp == NULL) {
		perror("Память не выделилась корректно\n");
		exit(2);
	}

	tmp->value = calloc(1, value_size);
	if (tmp->value == NULL) {
		perror("Память не выделилась корректно\n");
		exit(2);
	}

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
}

void* popBack(dblLinkedList* list) {
	Node* tmp;
	if (list->tail == NULL) {
		perror("Удалять нечего для popBack");
		exit(3);
	}
	tmp = list->tail;
	list->tail = list->tail->prev;
	if (list->tail) {
		list->tail->next = NULL;
	}
	if (tmp == list->head) {
		list->head = NULL;
	}
	void* value = tmp->value;
	free(tmp);
	return value;
}

void* popFront(dblLinkedList* list) {
	Node* tmp;
	if (list->head == NULL) {
		perror("Удалять нечего для popFront");
		exit(3);
	}
	tmp = list->head;
	list->head = list->head->next;
	if (list->head) {
		list->head->prev = NULL;
	}
	if (tmp == list->tail) {
		list->tail = NULL;
	}
	void* value = tmp->value;
	free(tmp);
	return value;
}

//	Чтобы работало для разных типов данных, надо описать функцию печати для этих типов

void printList(dblLinkedList* list, void(*print_func)(void*)) {
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
