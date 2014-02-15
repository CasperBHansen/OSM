/*
 * readwrite.c
 *
 * A test program of read and write.
 */

#include "proc/syscall.h"
#include "tests/lib.h"

//int main(int argc, char * argv[]) {
int main() {
    
    char buf[8];
    syscall_read(FILEHANDLE_STDIN, buf, sizeof(char) * 8);
    syscall_write(FILEHANDLE_STDOUT, buf, sizeof(char) * 8);

    //char *buf = "hej";

    return 0;
}

