#include "tests/lib.h"

static const int magic = 42;

void useStack() {
    int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    a[1] = a[1];

    syscall_memlimit(NULL);
}

int main()
{
    syscall_memlimit(NULL);

    useStack();

    return 0;
}
