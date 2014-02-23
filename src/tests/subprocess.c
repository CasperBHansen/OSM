/*
 * readwrite.c
 * 
 * A test program of read and write.
 */

#include "proc/syscall.h"
#include "tests/lib.h"

#define BUFFER_SIZE 8

//int main(int argc, char * argv[]) {
int main() {
      
    const char * hello_str = "Hello read/write!";
    syscall_write(FILEHANDLE_STDOUT, hello_str, sizeof(const char) * 18);
  
    return 1;
}
                                        
