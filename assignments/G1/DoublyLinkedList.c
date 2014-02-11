/*
 * DoublyLinkedList.c
 *
 * Defines the doubly linked list.
 */

#include "DoublyLinkedList.h"

#include <stdlib.h> // used for NULL definition
#include <stdio.h> // used for NULL definition

typedef unsigned long ptr_addr_t;

#define XOR_PTR(a, b) ((ptr_addr_t)a ^ (ptr_addr_t)b)

void insert(dlist *this, item *thing, bool atTail) {
    // initialize new node
    node *new = malloc(sizeof(node));
    new->thing = thing;

    if (!this->head) {                        // list is empty
        new->ptr = NULL;           // ptr XOR ptr = 0
        this->head = this->tail = new;

    } else if (this->head == this->tail) {      // singleton list
        this->head->ptr = this->tail->ptr = NULL;    // tail XOR tail = 0
        this->head = new;       // make head pointer point to new node

    } else {                                    // 2 or more elements
        // XOR'ed pointer for new node, i.e. this->head XOR this->tail
        node *new_ptr = (node *) XOR_PTR(this->head, this->tail);
        new->ptr = new_ptr; // set ptr field in new node

        // update pointers in head and tail nodes
        ptr_addr_t new_addr = (ptr_addr_t) new;
        this->head->ptr = (node *) (new_addr ^ XOR_PTR(this->head->ptr, this->tail));
        this->tail->ptr = (node *) (new_addr ^ XOR_PTR(this->tail->ptr, this->head));

        // make dlist head point to new node
        this->head = new;
    }
}

item * extract(dlist * this, bool atTail) {
    // node * loc = atTail ? this->tail : this->head;
    // needs implementation
    return NULL;
}

void reverse(dlist * this) {
    // needs implementation
}

item * search(dlist * this, bool (* matches(item *))) {

/* trying to print, for debugging. Atm figuring out why ptr's are wrong ..
	node * item = this->head;
	node * prev = NULL, * next = NULL;
	
	int i = 0;
	while (item != NULL) {
		
		printf("list[%i] := %lu: %d\n", i++, (ptr_addr_t)item, * (int *)(item->thing));
		
		next = (node *) XOR_PTR(prev, item->ptr);
		printf("%lu\n", (ptr_addr_t)next);
		
		//prev = item;
		item = NULL;
	}
	*/
    return NULL;
}
