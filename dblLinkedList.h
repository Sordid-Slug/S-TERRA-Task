#ifndef DBL_LINKED_LIST_H
#define DBL_LINKED_LIST_H

typedef struct _Node {
	struct _Node* prev;
	struct _Node* next;
	void* value;
} Node;

typedef struct _dblLinkedList {
	Node* head;
	Node* tail;
} dblLinkedList;

dblLinkedList *createList();
void deleteList(dblLinkedList** list);
void pushback(dblLinkedList** list, void* value, size_t value_size);
void* popBack(dblLinkedList* list);
void* popFront(dblLinkedList* list);

//	Чтобы работало для разных типов данных, надо описать функцию печати для этих типов, как printInt 

void printList(dblLinkedList *list, void(*print_func)(void*));
void printInt(void* value);
void printDouble(void* value);

#endif
