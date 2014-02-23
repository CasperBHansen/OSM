/*
 * subprocess.c
 * 
 * A test program of subprocesses.
 */

#include "proc/syscall.h"
#include "tests/lib.h"

#define BUFFER_SIZE 8

//int main(int argc, char * argv[]) {
int main() {
      
    const char * hello_str = "Hello read/write!";
    syscall_write(FILEHANDLE_STDOUT, hello_str, sizeof(const char) * 18);
    
    syscall_halt();
  
    return 1;
}
                                        
