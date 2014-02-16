/*
 * DoublyLinkedList.c
 *
 * Defines the doubly linked list.
 */

#include "DoublyLinkedList.h"

#include <stdlib.h>
#include <stdint.h> // uintptr_t for XOR'ing pointers
#include <stdio.h>

#define XOR_PTR(a, b) ((node *) ((uintptr_t) (a) ^ (uintptr_t) (b)))

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
        // XORed pointer is just this->tail since NULL XOR tail = tail
        new->ptr = this->tail;
        // this->tail->ptr contains the address of the node before tail
        this->tail->ptr = XOR_PTR(new, this->tail->ptr);
        this->tail = new;
    }
    // if we're appending to the head of the list
    else {
        // XORed pointer is just this->head since NULL XOR head = head
        new->ptr = this->head;
        // this->head->ptr contains the address of the node after head
        this->head->ptr = XOR_PTR(new, this->head->ptr);
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
        node * prev = XOR_PTR(n->ptr, NULL);
        if (!prev) this->head = NULL;
        else prev->ptr = XOR_PTR(n, XOR_PTR(prev->ptr, NULL));
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
        node * next = XOR_PTR(n->ptr, NULL);
        if (!next) this->tail = NULL;
        else next->ptr = XOR_PTR(n, XOR_PTR(next->ptr, NULL));
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

        next = XOR_PTR(prev, it->ptr);
        prev = it;
        it = next;
    }
    return NULL;
}

void print_int_list(dlist *this) {
    node *curr = this->head;
    node *prev = NULL, *next = NULL;

    while (curr != NULL) {
        printf("%d ", *(int *) curr->thing);
        next = XOR_PTR(prev, curr->ptr);
        prev = curr;
        curr = next;
    }
    printf("\n");
}
