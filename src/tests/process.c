#include "proc/syscall.h"
#include "tests/lib.h"
#include "lib/libc.h"

int main(void)
{
    int subp_pid = syscall_exec("[arkimedes]subprocess");

    char * str = "XX\n";
    syscall_write(FILEHANDLE_STDOUT, str, sizeof(const char) * 4);

    syscall_join(subp_pid);
    
    syscall_write(FILEHANDLE_STDOUT, str, sizeof(const char) * 4);
    
    return 0;
}
