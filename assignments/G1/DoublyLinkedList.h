/*
 * DoublyLinkedList.h
 *
 * Declares the doubly linked list per suggestion of the assignment text.
 */

#ifndef DLIST_H
#define DLIST_H

typedef int bool;
typedef void item; // we store pointers

typedef struct node {
    item * thing;
    struct node * ptr;
} node;

typedef struct dlist {
    node * head, * tail;
} dlist;

void insert(dlist *this, item *thing, bool atTail);
item * extract(dlist *this, bool atTail);
void reverse(dlist *this);
item * search(dlist *this, bool (*matches)(item *));

#endif
