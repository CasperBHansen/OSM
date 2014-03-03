#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stack.h"

#define NUM_THREADS 5

typedef struct {
    int * mat;
    int m, n; // lengths
} matrix_t;

typedef struct {
    matrix_t *lhs, *rhs;
    int i, j;
} task_t;

matrix_t * make_mat(int m, int n) {
    matrix_t * ret = (matrix_t *) malloc(sizeof(matrix_t));
    ret->m = m;
    ret->n = n;
    ret->mat = (int *)malloc(sizeof(int) * ret->m * ret->n);
    
    int i;
    for (i = 0; i < m * n; ++i)
        ret->mat[i] = rand() % 9;   // random number between 0 and 9
    
    return ret;
}

void print_matrix(matrix_t *mat) {
    int i;
    for (i = 0; i < mat->m * mat->n; i++) {
        if (i % mat->m == 0)
            printf("\n");
        printf("%3d ", mat->mat[i]);
    }
    printf("\n\n");
}

void *row_col_mult(void *arg) {
    task_t *task = (task_t *) arg;

    if (task->lhs->n != task->rhs->m) {
        printf("Matrices can not be multiplied.. Fail\n");
        exit(EXIT_FAILURE);
    }

    int len = task->lhs->n;
    int ret = 0;
    int x;

    for (x = 0; x < len; x++)
        ret += task->lhs->mat[(task->i * task->lhs->m) + x] *
               task->rhs->mat[task->j + (x * task->lhs->m)];

    pthread_exit((void *) ret); // replaces return
}

int main(int argc, char **argv) {
    pthread_t tids[NUM_THREADS];

    srand((unsigned long) time(NULL));

    stack_t *stack = malloc(sizeof(stack_t));
    stack_init(stack);

    matrix_t *a = make_mat(5,5);
    matrix_t *b = make_mat(5,5);

    printf("Matrix a:\n");
    print_matrix(a);

    printf("Matrix b:\n");
    print_matrix(b);

    // push row-column multiplication tasks onto stack
    int x, y;
    for (x = 0; x < a->n; x++) {
        for (y = 0; y < b->m; y++) {
            task_t *task = malloc(sizeof(task_t));
            task->lhs = a;
            task->rhs = b;
            task->i = x;
            task->j = y;
            stack_push(stack, task);
        }
    }

    task_t *task;
    int c;
    void *ret;

    matrix_t *res_mat = malloc(sizeof(matrix_t));
    res_mat->m = a->n;
    res_mat->n = b->m;
    res_mat->mat = (int *) malloc(sizeof(int) * res_mat->m * res_mat->n);

    task_t *tasks[NUM_THREADS];

    while(! stack_empty(stack)) {
        c = 0;
        while(c < NUM_THREADS && (task = (task_t *) stack_pop(stack)) != NULL)
            tasks[c++] = task;

        for (c = 0; c < NUM_THREADS; c++)
            pthread_create(&tids[c], NULL, &row_col_mult, tasks[c]);

        for (c = 0; c < NUM_THREADS; c++) {
            pthread_join(tids[c], (void **) &ret);
            res_mat->mat[res_mat->m * tasks[c]->i + tasks[c]->j] = ret;
            free(tasks[c]);
        }
    }

    printf("Matrix a * b:\n");
    print_matrix(res_mat);

    stack_destroy(stack);
    free(stack);
    stack = NULL;

    return 0;
}
