#include "tests/lib.h"
#include "lib/libc.h"

int main(void)
{
    int subp_pid = syscall_exec("[arkimedes]subprocess");

    int subp_retval = syscall_join(subp_pid);

    subp_retval = subp_retval;

    return 0;
}
