#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int main() {
    stack_t *stack = malloc(sizeof(stack_t));

    stack_init(stack);

    int element = 42;

    stack_push(stack, &element);

    if (stack_empty(stack))
        printf("Stack is empty!\n");
    else {
        printf("Stack top contains: %d\n", *((int *) stack_top(stack)));
        int popped_val = *((int *) stack_pop(stack));
        printf("Popped element: %d\n", popped_val);
    }

    if (stack_empty(stack))
        printf("Stack is empty!\n");

    free(stack); // clean up

    return 0;
}
