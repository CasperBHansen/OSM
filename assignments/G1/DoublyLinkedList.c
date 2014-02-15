/*
 * DoublyLinkedList.c
 *
 * Defines the doubly linked list.
 */

#include "DoublyLinkedList.h"

#include <stdlib.h>

typedef unsigned long ptr_addr_t;

#define XOR_PTR(a, b) ((ptr_addr_t)a ^ (ptr_addr_t)b)

void insert(dlist *this, item *thing, bool atTail) {
    // initialize new node
    node *new = malloc(sizeof(node));
    new->thing = thing;
	
	// empty list
    if (!this->head) {
        new->ptr = NULL;
        this->head = this->tail = new;
    }
	// if we're appending to the tail of the list
	else if (atTail) {
		new->ptr = (node *) XOR_PTR(this->tail, NULL);
		this->tail->ptr = (node *) XOR_PTR(new, XOR_PTR(this->tail->ptr, NULL));
		this->tail = new;
	}
	// if we're appending to the head of the list
	else {
		new->ptr = (node *) XOR_PTR(this->head, NULL);
		this->head->ptr = (node *) XOR_PTR(new, XOR_PTR(this->head->ptr, NULL));
		this->head = new;
	}
}

item * extract(dlist * this, bool atTail) {
    item * ret = NULL;
    if (!this->head) {
        return NULL;
    }
    else if (atTail) {
        // retrieve the item
        node * n = this->tail;
        ret = n->thing;
        // correct the head and tail
        node * prev = (node *) XOR_PTR(n->ptr, NULL);
        if (!prev) this->head = NULL;
        else prev->ptr = (node *) XOR_PTR(n, XOR_PTR(prev->ptr, NULL));
        this->tail = prev;
        // deallocate the node
        if (n) free(n);
        n = NULL;
    }
    else {
        // retrieve the item
        node * n = this->head;
        ret = n->thing;
        // correct the head and tail
        node * next = (node *)XOR_PTR(n->ptr, NULL);
        if (!next) this->tail = NULL;
        else next->ptr = (node *) XOR_PTR(n, XOR_PTR(next->ptr, NULL));
        this->head = next;
        // deallocate the node
        if (n) free(n);
        n = NULL;
    }
    return ret;
}

void reverse(dlist * this) {
    node * tmp = this->head;
	this->head = this->tail;
	this->tail = tmp;
}

item * search(dlist * this, bool (* matches)(item *)) {
	
	node * it = this->head;
	node * prev = NULL, * next = NULL;
	
	while (it != NULL) {
		if (matches(it->thing))
			return it->thing;
		
		next = (node *) XOR_PTR(prev, it->ptr);
		prev = it;
		it = next;
	}
	
    return NULL;
}
