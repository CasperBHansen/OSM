/*
 * readwrite.c
 *
 * A test program of read and write.
 */

#include "proc/syscall.h"
#include "tests/lib.h"

int main(int argc, char * argv[]) {
    
    char buf;
    syscall_read(FILEHANDLE_STDIN, &buf, sizeof(char));
    
    return 0;
}

