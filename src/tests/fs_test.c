#include "lib.h"

// Macros largely inspired by http://c.learncodethehardway.org/book/ex20.html
#define log_err(M, ...) printf("[ERROR] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_assert(M, ...) printf("[ASSERT SUCCESS] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) log_err(M, ##__VA_ARGS__);
#define assert(A, M, ...) if(A) log_assert(M, ##__VA_ARGS__); else log_err(M, ##__VA_ARGS__);

int main()
{
    const char *pathname = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                           "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                           "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                           "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                           "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                           "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    // This call to syscall_open should return a negative value, indicating an error,
    // since the pathname is too long.
    assert(syscall_open(pathname) < 0,
           "Filepath %s is too long: %d", pathname, strlen(pathname));

    int handle0 = syscall_open("[arkimedes]fs_test");
    int handle1 = syscall_open("[arkimedes]fs_test");
    int handle2 = syscall_open("[arkimedes]fs_test");

    assert(handle0 == 3,
           "Filehandle %d returned. The first handle should be 3 since "
           "std{in,out,err} are 0, 1 and 2.", handle0);

    assert(handle1 == 4,
           "Filehandle %d returned. The second handle should be 4.", handle1);

    assert(handle2 == 5,
           "Filehandle %d returned. The third handle should be 5.", handle2);

    int close_ret = syscall_close(handle2);
    assert(close_ret == 0,
           "Filehandle %d should be closeable. syscall_close(%d) returns %d.",
           handle2, handle2, close_ret);

    close_ret = syscall_close(42); // not in process PCB
    assert(close_ret != 0,
           "File handle 42 should not be open in current process. "
           "syscall_close(42) returns %d.", close_ret);

    char *new_file = "[arkimedes]new_file";
    int new_file_size = 8;
    int create_ret = syscall_create(new_file, new_file_size);
    assert(create_ret == 0,
           "New file with pathname %s, of size %d, should be creatable "
           "(if there is enough space).\nReturn value: %d.",
           new_file, new_file_size, create_ret);

    /*int delete_ret =*/
    //syscall_delete(new_file);
    /*assert(delete_ret == 0,
           "File with pathname %s should be deletable if the previous "
           "syscall_create was successful.\nReturn value %d.",
           new_file, delete_ret);*/


    //syscall_halt();

    return 0;
}
