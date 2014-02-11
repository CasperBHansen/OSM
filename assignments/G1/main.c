/*
 * main.c
 *
 * the main program.
 */

#include <stdlib.h>
#include <stdio.h>

#include "DoublyLinkedList.h"

#define NUM_ITEMS 2

int things[NUM_ITEMS] = { 1, 2 };

// not used at this time
bool * match_int(void * arg)
{
	return NULL;
}

int main(int argc, char * argv[]) {

    dlist *list = malloc(sizeof(dlist));
	
	int i = 0;
	while (i < NUM_ITEMS) insert(list, &things[i++], 0);
	
	printf("head [addr: %lu ptr: %lu] %d\n", (long unsigned)list->head, (long unsigned)list->head->ptr, * (int *)list->head->thing);
    printf("tail [addr: %lu ptr: %lu] %d\n", (long unsigned)list->tail, (long unsigned)list->tail->ptr, * (int *)list->tail->thing);
	
	search(list, &match_int);
	
	return EXIT_SUCCESS;
}
