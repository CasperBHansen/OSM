#ifndef STACK_H
#define STACK_H

/* Singly-linked list data structure for storing the dynamic stack. */
typedef struct list_t {
    void *head;
    struct list_t *tail;
} list_t;

/* Dynamically allocated limitless stack implemented using a singly-linked list.
 * The structure member 'top' is a pointer to the list containing the elements
 * on the stack; the head of the list is a void pointer to the top element on
 * the stack. If top is NULL, the stack is empty. */
typedef struct stack_t {
    list_t *top;
} stack_t;

/* Initialize a freshly allocated stack.
 * Must be called before using any of the other stack functions.*/
void stack_init(stack_t *stack);

/* Returns true (not 0) if the stack is empty. */
int stack_empty(stack_t *stack);

/* Destroy the stack: free the memory and destroy the mutex lock. */
void stack_destroy(stack_t *stack);

/* Return the top element of the stack.
 * Returns NULL if the stack is empty. */
void *stack_top(stack_t *stack);

/* Remove the top element of the stack and return it.
 * Returns NULL if the stack is empty. */
void *stack_pop(stack_t *stack);

/* Push an element to the top of the stack. Returns 0 if possible, any other
 * value if there was an error (for example, if the stack is full or no memory
 * could be allocated). */
int stack_push(stack_t *stack, void *elem);

#endif
