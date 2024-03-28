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
	int size;
} dblLinkedList;

#endif