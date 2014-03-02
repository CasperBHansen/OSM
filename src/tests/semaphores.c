/*
 * semaphores.c
 *
 * Test case of userland semaphores.
 */

#include "tests/lib.h"

const char str[] = "Hello Semaphores!\n";

int main() {
    
    syscall_write(FILEHANDLE_STDOUT, str, sizeof(str));
    
    return 0;
}
