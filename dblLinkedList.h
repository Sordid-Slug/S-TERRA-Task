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
	size_t size;
} dblLinkedList;

dblLinkedList *createList();
void deleteList(dblLinkedList** list);
void pushback(dblLinkedList** list, void* value, size_t value_size);
Node* getNode(dblLinkedList* list, size_t index);
int delNode(dblLinkedList* list, size_t index);
int popBack(dblLinkedList* list);
int popFront(dblLinkedList* list);

//	Чтобы работало для разных типов данных, надо описать функцию печати для этих типов, как printInt 

void printList(dblLinkedList *list, void(*print_func)(void*));
void printInt(void* value);
void printDouble(void* value);

#endif