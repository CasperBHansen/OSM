#include <stdlib.h>
#include "stack.h"

void stack_init(stack_t *stack) {
    stack->top = NULL;
}

int stack_empty(stack_t *stack) {
    return stack->top == NULL;
}

void *stack_top(stack_t *stack) {
    if (stack_empty(stack))
        return NULL;
    return stack->top->head;
}

void *stack_pop(stack_t *stack) {
    if (stack_empty(stack))
        return NULL;

    void *ret = stack->top->head;
    list_t *tmp = stack->top;
    stack->top = stack->top->tail;

    free(tmp);

    return ret;
}

int stack_push(stack_t *stack, void *elem) {
    list_t *new_elem = malloc(sizeof(list_t));
    if (new_elem == NULL)
        return -1; // memory could not be allocated

    new_elem->head = elem;
    new_elem->tail = stack->top;
    stack->top = new_elem;

    return 0;
}
