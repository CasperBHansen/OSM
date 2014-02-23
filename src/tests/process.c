#include "tests/lib.h"

int main(void)
{
    syscall_exec("[arkimedes]readwrite");
    
    int i;
    for (i = 0; i < 10000; ++i) {
    }

    return 0;
}
