/*
 * main.c
 *
 * the main program.
 */

#include <stdlib.h>
#include <stdio.h>

#include "DoublyLinkedList.h"

int main(int argc, char * argv[]) {
	
	printf("Hello, doubly-linked list!");
	
	dlist list;
	insert(&list, (item *)2, 0);
	insert(&list, (item *)4, 0);
	insert(&list, (item *)8, 0);
	insert(&list, (item *)16, 0);
	
	return EXIT_SUCCESS;
}

