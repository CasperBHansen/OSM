/*
 * main.c
 *
 * the main program.
 */

#include <stdlib.h>
#include <stdio.h>

#include "DoublyLinkedList.h"

#define NUM_ITEMS 6

int things[NUM_ITEMS] = { 1, 2, 4, 8, 16, 32 };

bool match_int(item * arg) {
    return (* (int *) arg == 4) ? 1 : 0; // true or false
}

int main(int argc, char * argv[]) {

    dlist *list = malloc(sizeof(dlist));

    // TEST: insert
    printf("Inserting..\n");
    int i = 0;
    while (i < NUM_ITEMS) insert(list, &things[i++], 0);

    // TEST: print_int_list
    printf("Print int list:\n");
    print_int_list(list);

    // TEST: search
    printf("Searching..\n");
    item * match = NULL;
    match = search(list, match_int);
    if (match) printf("Item %d found\n", * (int *)match);

    // TEST: reverse
    printf("Reversing..\n");
    reverse(list);

    printf("Print int list:\n");
    print_int_list(list);

    match = search(list, match_int);
    if (match) printf("Item %d found\n", * (int *)match);

    // TEST: extract
    printf("Extracting..\n");
    item * thing = NULL;
    while (list->head) {
        thing = extract(list, 0);
        printf("%i\n", * (int *)(thing));
    }

    free(list);

    return EXIT_SUCCESS;
}
