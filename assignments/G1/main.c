/*
 * main.c
 *
 * the main program.
 */

#include <stdlib.h>
#include <stdio.h>

#include "DoublyLinkedList.h"

int main(int argc, char * argv[]) {

    int things[] =  { 2, 4, 8, 16 };
    
    printf("Hello, doubly-linked list!\n");

    dlist *list = malloc(sizeof(dlist));

    insert(list, &things[0], 0);
    insert(list, &things[1], 0);
    insert(list, &things[2], 0);
    insert(list, &things[3], 0);

    printf("list->head->thing: %d\n", *((int *) list->head->thing));
    printf("list->tail->thing: %d\n", *((int *) list->tail->thing));

    return EXIT_SUCCESS;
}
