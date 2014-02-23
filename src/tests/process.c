#include "proc/syscall.h"
#include "tests/lib.h"
#include "lib/libc.h"

int main(void)
{
    int subp_pid = syscall_exec("[arkimedes]readwrite");

    const char * hello_str = "XX\n";
    syscall_write(FILEHANDLE_STDOUT, hello_str, sizeof(const char) * 4);

    int subp_retval = syscall_join(subp_pid);
    subp_retval = subp_retval;
    const char * ret = "12345678";
    syscall_write(FILEHANDLE_STDOUT, ret, 9);
    
    return 0;
}
