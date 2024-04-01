#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dblLinkedList.h"

dblLinkedList* createList() {
	dblLinkedList* tmp = (dblLinkedList*)calloc(1, sizeof(dblLinkedList));
	if (!tmp) {
		printf("Память не выделилась корректно\n");
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
		printf("Память не выделилась корректно\n");
		exit(2);
	}

	tmp->value = calloc(1, value_size);
	if (tmp->value == NULL) {
		printf("Память не выделилась корректно\n");
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
	(*list)->size++;
}

Node* getNode(dblLinkedList* list, size_t index) {
	if (index > list->size) {
		printf("Индекс больше возможного");
		exit(4);
	}
	Node* tmp = NULL;
	if (index < list->size / 2) {
		tmp = list->head;
		for (size_t i = 0; i < index && tmp; i++) {
			tmp = tmp->next;
		}
	}
	else {
		tmp = list->tail;
		for (size_t i = list->size - 1; i > index && tmp; i--) {
			tmp = tmp->prev;
		}
	}

	return tmp;
}

int popBack(dblLinkedList* list) {
	Node* tmp;

	if (list->tail == NULL) {
		printf("Удалять нечего для popBack");
		return 1;
	}
	tmp = list->tail;
	list->tail = list->tail->prev;
	if (list->tail) {
		list->tail->next = NULL;
	}
	if (tmp == list->head) {
		list->head = NULL;
	}
	__sync_fetch_and_sub(&list->size, 1);
	free(tmp->value);
	free(tmp);
	if (list->size == 0) {
		list->head = NULL;
		list->tail = NULL;
	}
	return 0;
}

int popFront(dblLinkedList* list) {
	Node* tmp;

	if (list->head == NULL) {
		printf("Удалять нечего для popBack");
		return 1;
	}
	tmp = list->head;
	list->head = list->head->next;
	if (list->head) {
		list->head->prev = NULL;
	}
	if (tmp == list->tail) {
		list->tail = NULL;
	}
	__sync_fetch_and_sub(&list->size, 1);
	free(tmp->value);
	free(tmp);
	if (list->size == 0) {
		list->head = NULL;
		list->tail = NULL;
	}
	return 0;
}

int delNode(dblLinkedList* list, size_t index) {
	Node* elm = getNode(list, index);
	if (elm == NULL) {
		printf("Удалять нечего для popBack");
		return 1;
	}
	if (elm->prev) {
		elm->prev->next = elm->next;
	}
	if (elm->next) {
		elm->next->prev = elm->prev;
	}
	if (!elm->prev) {
		list->head = elm->next;
	}
	if (!elm->next) {
		list->tail = elm->prev;
	}
	__sync_fetch_and_sub(&list->size, 1);
	free(elm->value);
	free(elm);
	return 0;
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