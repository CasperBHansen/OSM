#include "tests/lib.h"

int main()
{
    heap_init();
    
    int n = 4;
    int * ints = (int *)malloc(sizeof(int) * n);
    ints = ints;
    
    int i;
    for (i = 0; i < n; ++i)
        ints[i] = i * 2;
    
    for (i = 0; i < n; ++i)
        printf("ints[%i] = %i\n", i, ints[i]);
    
    free(ints);
    
    return 0;
}
