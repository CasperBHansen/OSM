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

bool match_int(item * arg) {
    return (* (int *) arg == 4) ? 1 : 0; // true or false
}

int main(int argc, char * argv[]) {

    dlist *list = malloc(sizeof(dlist));

    int i = 0;
    while (i < NUM_ITEMS) insert(list, &things[i++], 0);

    item * match = NULL;
    match = search(list, match_int);
    if (match) printf("Item %d found\n", * (int *)match);

    reverse(list);
    match = search(list, match_int);
    if (match) printf("Item %d found\n", * (int *)match);

    printf("Extracting..\n");
    item * thing = NULL;
    while (list->head) {
        thing = extract(list, 0);
        printf("%i\n", * (int *)(thing));
    }

    free(list);

    return EXIT_SUCCESS;
}
