#include "proc/syscall.h"
#include "tests/lib.h"
#include "lib/libc.h"

int main(void)
{
    char *exec_str = "\n\nPerforming exec of subprocess..\n";
    syscall_write(FILEHANDLE_STDOUT, exec_str, sizeof(char) * 35);

    int subp_pid = syscall_exec("[arkimedes]subprocess");

    char *exec_str1 = "\n\nReturned from exec syscall..\n";
    syscall_write(FILEHANDLE_STDOUT, exec_str1, sizeof(char) * 32);

    char *join_str = "\n\nPerforming join with subprocess..\n";
    syscall_write(FILEHANDLE_STDOUT, join_str, sizeof(char) * 37);

    syscall_join(subp_pid);
    
    char *done_str = "\n\nDone with join, returning..\n";
    syscall_write(FILEHANDLE_STDOUT, done_str, sizeof(char) * 31);
    
    return 0;
}
