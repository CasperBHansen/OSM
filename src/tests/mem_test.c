#include "tests/lib.h"

int main()
{
    const int ints_alloc  = 42;
    const int chars_alloc = 35;

    heap_init();

    int *ints   = malloc(sizeof(int)  * ints_alloc);
    char *chars = malloc(sizeof(char) * chars_alloc);

    int i = 0;
    for (i = 0; i < ints_alloc; i++)
        ints[i] = i * 2;

    for (i = 0; i < chars_alloc; i++)
        chars[i] = i + 32;

    for (i = 0; i < ints_alloc; i++)
        printf("ints[%d]: %d\n", i, ints[i]);

    for (i = 0; i < chars_alloc; i++)
        printf("chars[%d]: %c\n", i, chars[i]);

    printf("freeing ints...\n");
    free(ints);

    for (i = 0; i < chars_alloc; i++)
        printf("chars[%d]: %c\n", i, chars[i]);
    
    printf("freeing chars...\n");
    free(chars);

    /*printf("malloc ints...\n");
    int *ints = malloc(sizeof(int) * 8);

    printf("freeing ints...\n");
    free(ints);

    printf("malloc ints2...\n");
    int *ints2 = malloc(sizeof(int) * 8);

    printf("freeing ints...\n");
    free(ints2);*/


    /*heap_init();
    
    int n = 16;
    int * ints = (int *)malloc(sizeof(int) * n);
    ints = ints;
    
    int i;
    for (i = 0; i < n; ++i)
        ints[i] = i * 2;
    
    for (i = 0; i < n; ++i)
        printf("ints[%i] = %i\n", i, ints[i]);
    
    free(ints);*/
    
    return 0;
}
