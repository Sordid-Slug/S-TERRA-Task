#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dblLinkedList.h"

dblLinkedList *createList() {
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
	} else {
		tmp = list->tail;
		for (size_t i = list->size - 1; i > index && tmp; i--) {
			tmp = tmp->prev;
		}
	}
	return tmp;
}

void* popBack(dblLinkedList* list) {
	Node *tmp;
	void *res;
	
	if (list->tail == NULL) {
		printf("Удалять нечего");
		exit(6);
	}
	tmp = list->tail;
	list->tail = list->tail->prev;
	if (list->tail) {
		list->tail->next = NULL;
	}
	if (tmp == list->head) {
		list->head = NULL;
	}
	res = tmp->value;
	list->size--;
	free(tmp);
	return res;
}

void* popFront(dblLinkedList* list) {
	Node *tmp;
	void *res;
	
	if (list->head == NULL) {
		printf("Удалять нечего");
		exit(6);
	}
	tmp = list->head;
	list->head = list->head->next;
	if (list->head) {
		list->head->prev = NULL;
	}
	if (tmp == list->tail) {
		list->tail = NULL;
	}
	res = tmp->value;
	list->size--;
	free(tmp);
	return res;
}

void* delNode(dblLinkedList* list, size_t index) {
	Node* elm = getNode(list, index);
	if (elm == NULL) {
        printf("Попытка удаления несуществующего элемента"); 
		exit(3);
	}
	if (elm->prev) {
		elm->prev->next = elm->next;
	}
	if (elm->next) {
		elm->next->prev = elm->prev;
	}
	void *tmp = elm->value;
	if (!elm->prev) {
		list->head = elm->next;
	}
	if (!elm->next) {
		list->tail = elm->prev;
	}
	free(elm);
	list->size--;

	return tmp;
}

//	Чтобы работало для разных типов данных, надо описать функцию печати для этих типов

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