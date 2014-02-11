/*
 * main.c
 *
 * the main program.
 */

#include <stdlib.h>
#include <stdio.h>

#include "DoublyLinkedList.h"

#define NUM_ITEMS 4

int things[NUM_ITEMS] = { 1, 2, 4, 8 };

bool * match_int(item * arg)
{
	return (* (int *)arg == 4) ? arg : NULL;
}

int main(int argc, char * argv[]) {

    dlist *list = malloc(sizeof(dlist));
	
	int i = 0;
	while (i < NUM_ITEMS) insert(list, &things[i++], 0);
	
	item * match = search(list, &match_int);
	reverse(list);
	search(list, &match_int);
	if (match)
		printf("Item %d found\n", * (int *)match);
	
	return EXIT_SUCCESS;
}
