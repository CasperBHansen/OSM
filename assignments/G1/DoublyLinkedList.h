/*
 * DoublyLinkedList.h
 *
 * Declares the doubly linked list per suggestion of the assignment text.
 */

typedef int bool;
typedef void item; // we store pointers

typedef struct node_ {
	item * thing;
	struct node_ * ptr;
} node;

typedef struct dlist_ {
	node * head, * tail;
} dlist;

void insert(dlist * this, item * thing, bool atTail);
item * extract(dlist * this, bool atTail);
void reverse(dlist * this);
item * search(dlist * this, bool (* matches(item *)));

