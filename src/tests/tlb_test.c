/*
 * tlb_test.c
 */

#include "tests/lib.h"

int main() {
    char *buffer;

    // initialize heap
    printf("Initializing heap..\n");
    heap_init();

    printf("Heap size is %d bytes\n", HEAP_SIZE);

    printf("Allocating 200 bytes of heap space..\n");
    buffer = malloc(sizeof(char) * 200);

    printf("Setting allocated memory to 'A' bytes..\n");
    memset(buffer, (int) 'A', 200);

    printf("Freeing memory..\n");
    free(buffer);


    /*printf("Allocating 300 bytes of heap space..\n");
    buffer = malloc(sizeof(char) * 300);

    printf("Setting allocated memory to 'B' bytes..\n");
    memset(buffer, (int) 'B', 300);

    printf("Freeing memory..\n");
    free(buffer);*/

    return 0;
}
