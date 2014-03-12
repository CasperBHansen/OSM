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

    return 0;
}
