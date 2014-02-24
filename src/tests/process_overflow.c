#include "proc/syscall.h"
#include "tests/lib.h"
#include "lib/libc.h"

int main(void)
{
    int subp0_pid  = syscall_exec("[arkimedes]subprocess");
    int subp1_pid  = syscall_exec("[arkimedes]subprocess");
    int subp2_pid  = syscall_exec("[arkimedes]subprocess");
    int subp3_pid  = syscall_exec("[arkimedes]subprocess");
    int subp4_pid  = syscall_exec("[arkimedes]subprocess");
    int subp5_pid  = syscall_exec("[arkimedes]subprocess");
    int subp6_pid  = syscall_exec("[arkimedes]subprocess");
    int subp7_pid  = syscall_exec("[arkimedes]subprocess");
    int subp8_pid  = syscall_exec("[arkimedes]subprocess");
    int subp9_pid  = syscall_exec("[arkimedes]subprocess");
    int subp10_pid = syscall_exec("[arkimedes]subprocess");
    int subp11_pid = syscall_exec("[arkimedes]subprocess");
    int subp12_pid = syscall_exec("[arkimedes]subprocess");
    int subp13_pid = syscall_exec("[arkimedes]subprocess");
    int subp14_pid = syscall_exec("[arkimedes]subprocess");
    int subp15_pid = syscall_exec("[arkimedes]subprocess");
    int subp16_pid = syscall_exec("[arkimedes]subprocess");
    int subp17_pid = syscall_exec("[arkimedes]subprocess");
    int subp18_pid = syscall_exec("[arkimedes]subprocess");
    int subp19_pid = syscall_exec("[arkimedes]subprocess");
    int subp20_pid = syscall_exec("[arkimedes]subprocess");
    int subp21_pid = syscall_exec("[arkimedes]subprocess");
    int subp22_pid = syscall_exec("[arkimedes]subprocess");
    int subp23_pid = syscall_exec("[arkimedes]subprocess");
    int subp24_pid = syscall_exec("[arkimedes]subprocess");
    //int subp25_pid = syscall_exec("[arkimedes]subprocess");
    //int subp26_pid = syscall_exec("[arkimedes]subprocess");
    //int subp27_pid = syscall_exec("[arkimedes]subprocess");
    //int subp28_pid = syscall_exec("[arkimedes]subprocess");
    //int subp29_pid = syscall_exec("[arkimedes]subprocess");
    //int subp30_pid = syscall_exec("[arkimedes]subprocess");

    subp0_pid = subp0_pid;
    subp1_pid = subp1_pid;
    subp2_pid = subp2_pid;
    subp3_pid = subp3_pid;
    subp4_pid = subp4_pid;
    subp5_pid = subp5_pid;
    subp6_pid = subp6_pid;
    subp7_pid = subp7_pid;
    subp8_pid = subp8_pid;
    subp9_pid = subp9_pid;
    subp10_pid = subp10_pid;
    subp11_pid = subp11_pid;
    subp12_pid = subp12_pid;
    subp13_pid = subp13_pid;
    subp14_pid = subp14_pid;
    subp15_pid = subp15_pid;
    subp16_pid = subp16_pid;
    subp17_pid = subp17_pid;
    subp18_pid = subp18_pid;
    subp19_pid = subp19_pid;
    subp20_pid = subp20_pid;
    subp21_pid = subp21_pid;
    subp22_pid = subp22_pid;
    subp23_pid = subp23_pid;
    subp24_pid = subp24_pid;
    //subp25_pid = subp25_pid;
    //subp26_pid = subp26_pid;
    //subp27_pid = subp27_pid;
    //subp28_pid = subp28_pid;
    //subp29_pid = subp29_pid;
    //subp30_pid = subp30_pid;

    char * str = "XX\n";
    syscall_write(FILEHANDLE_STDOUT, str, sizeof(const char) * 4);

    syscall_join(subp0_pid);
    
    syscall_write(FILEHANDLE_STDOUT, str, sizeof(const char) * 4);
    
    return 0;
}
