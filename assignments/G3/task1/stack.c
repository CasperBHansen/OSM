#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

// declaration of globally accessible mutex
pthread_mutex_t mutex;

void stack_init(stack_t *stack) {
    stack->top = NULL;

    // initialize pthread mutex with default mutex attributes; 
    // a pthread mutex is initially unlocked
    int tmp = pthread_mutex_init(&mutex, NULL);
    if (tmp != 0) {
        printf("Error: Pthread mutex could not be initialized, \
                error number %d returned.\n", tmp);
        exit(EXIT_FAILURE);
    }
}

void stack_destroy(stack_t *stack) {
    // lock mutex; if the mutex is already locked by another thread, the
    // calling thread is blocked until the mutex is unlocked.
    int lock_tmp = pthread_mutex_lock(&mutex);
    if (lock_tmp != 0) {
        printf("Error: Pthread mutex could not be locked, \
                error number %d returned.\n", lock_tmp);
        exit(EXIT_FAILURE);
    }

    list_t *node = stack->top;
    list_t *tmp;

    // free memory of the whole list
    while (node) {
        tmp = node;
        node = node->tail;
        free(tmp);
    }

    // unlock mutex
    int unlock_tmp = pthread_mutex_unlock(&mutex);
    if (unlock_tmp != 0) {
        printf("Error: Pthread mutex could not be unlocked, \
                error number %d returned.\n", unlock_tmp);
        exit(EXIT_FAILURE); // this should not happen since a lock was acquired
    }
    
    // destroy the mutex lock
    int mutex_tmp = pthread_mutex_destroy(&mutex);
    if (mutex_tmp != 0) {
        printf("Error: Pthread mutex could not be destroyed, \
                error number %d returned.\n", mutex_tmp);
        exit(EXIT_FAILURE);
    }
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

    // lock mutex; if the mutex is already locked by another thread, the
    // calling thread is blocked until the mutex is unlocked.
    int lock_tmp = pthread_mutex_lock(&mutex);
    if (lock_tmp != 0) {
        printf("Error: Pthread mutex could not be locked, \
                error number %d returned.\n", lock_tmp);
        exit(EXIT_FAILURE);
    }

    void *ret = stack->top->head;
    list_t *tmp = stack->top;
    stack->top = stack->top->tail;

    free(tmp);

    // unlock mutex
    int unlock_tmp = pthread_mutex_unlock(&mutex);
    if (unlock_tmp != 0) {
        printf("Error: Pthread mutex could not be unlocked, \
                error number %d returned.\n", unlock_tmp);
        exit(EXIT_FAILURE); // this should not happen since a lock was acquired
    }

    return ret;
}

int stack_push(stack_t *stack, void *elem) {
    // lock mutex; if the mutex is already locked by another thread, the
    // calling thread is blocked until the mutex is unlocked.
    int lock_tmp = pthread_mutex_lock(&mutex);
    if (lock_tmp != 0)
        return -1;

    list_t *new_elem = malloc(sizeof(list_t));
    if (new_elem == NULL)
        return -1; // memory could not be allocated

    new_elem->head = elem;
    new_elem->tail = stack->top;
    stack->top = new_elem;

    // unlock mutex
    int unlock_tmp = pthread_mutex_unlock(&mutex);
    if (unlock_tmp != 0) {
        printf("Error: Pthread mutex could not be unlocked, \
                error number %d returned.\n", unlock_tmp);
        exit(EXIT_FAILURE); // this should not happen since a lock was acquired
    }

    return 0;
}
