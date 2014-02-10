/*
 * DoublyLinkedList.c
 *
 * Defines the doubly linked list.
 */

#include "DoublyLinkedList.h"

#include <stdlib.h> // used for NULL definition

void insert(dlist * this, item * thing, bool atTail) {
	// node * loc = atTail ? this->tail : this->head;
	// needs implementation

    // initialize new node
    node *new = malloc(sizeof(node));
    new->thing = thing;

    if (!(this->head)) {                        // list is empty
        new->ptr = 0;           // ptr XOR ptr = 0
        this->head = new;       // make head and tail point to new node
        this->tail = new;

    } else if (this->head == this->tail) {      // singleton list
        this->head->ptr = 0;    // head XOR head = 0
        this->tail->ptr = 0;    // tail XOR tail = 0
        this->head = new;       // make head pointer point to new node

    } else {                                    // 2 or more elements
        // XOR'ed pointer for new node, i.e. this->head XOR this->tail
        long head_addr = (long) this->head;
        long tail_addr = (long) this->tail;
        node *new_ptr = (node *) (head_addr ^ tail_addr);

        new->ptr = new_ptr; // set ptr field in new node

        // update pointers in head and tail nodes
        long head_ptr = (long) this->head->ptr;
        long secfirst_node_addr = head_ptr ^ tail_addr;

        long tail_ptr = (long) this->tail->ptr;
        long seclast_node_addr = tail_ptr ^ head_addr;

        long new_addr = (long) new;

        this->head->ptr = (node *) (new_addr ^ secfirst_node_addr);
        this->tail->ptr = (node *) (new_addr ^ seclast_node_addr);

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
	// needs implementation
	return NULL;
}

