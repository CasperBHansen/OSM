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
  
    // read 8 bytes, wait for each character
    char buf[BUFFER_SIZE];
    int i = 0;
    while (i < BUFFER_SIZE) {
        syscall_read(FILEHANDLE_STDIN, buf + i, sizeof(char));
        ++i;
    }
                            
    syscall_write(FILEHANDLE_STDOUT, buf, sizeof(char) * BUFFER_SIZE);
    
    return 90;
}
                                        
