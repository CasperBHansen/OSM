/*
 * subprocess.c
 * 
 * A test program of subprocesses.
 */

#include "proc/syscall.h"
#include "tests/lib.h"

int main() {
      
    const char * hello_str = "Hello sub-process!";
    syscall_write(FILEHANDLE_STDOUT, hello_str, sizeof(const char) * 18);
  
    return 1;
}
                                        
