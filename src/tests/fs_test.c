#include "lib.h"

// Macros largely inspired by http://c.learncodethehardway.org/book/ex20.html
#define log_err(M, ...) printf("[ERROR] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_assert(M, ...) printf("[ASSERT SUCCESS] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) log_err(M, ##__VA_ARGS__);
#define assert(A, M, ...) if(A) log_assert(M, ##__VA_ARGS__); else log_err(M, ##__VA_ARGS__);

int main()
{
    const char *hello = "Hello, Filesystem!";

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
    
    assert(syscall_close(0) != 0,
            "Filehandle %d should not be closable.", 0);

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

    syscall_close(handle0);
    syscall_close(handle1);

    close_ret = syscall_close(42); // not in process PCB
    assert(close_ret != 0,
           "File handle 42 should not be open in current process. "
           "syscall_close(42) returns %d.", close_ret);

    char *new_file = "[arkimedes]new_file";
    int new_file_size = 32;
    int create_ret = syscall_create(new_file, new_file_size);
    assert(create_ret == 0,
           "New file with pathname %s, of size %d, should be creatable "
           "(if there is enough space).\nReturn value: %d.",
           new_file, new_file_size, create_ret);

    int new_handle = syscall_open(new_file);


    int seek_ret = syscall_seek(new_handle, 4);
    assert(seek_ret == 0,
           "Seeking in %s by 4 bytes.", new_file);

    seek_ret = syscall_seek(new_handle, new_file_size - 1);
    assert(seek_ret == 0,
           "Seeking in %s by %d bytes.", new_file, new_file_size - 1);

    seek_ret = syscall_seek(new_handle, new_file_size);
    assert(seek_ret != 0,
           "Seeking in %s by %d bytes, should fail.", new_file, new_file_size);

    syscall_seek(new_handle, 0);

    int write_ret = syscall_write(new_handle, hello, strlen(hello));
    assert(write_ret == (int) strlen(hello),
           "Writing \"%s\" to file handle %d, returns: %d",
           hello, new_handle, write_ret);

    syscall_seek(new_handle, 0);

    char buffer[18];
    int read_ret = syscall_read(new_handle, buffer, 18);
    assert(read_ret != -1,
           "Read %d bytes (return value) from file handle %d, buffer contains \"%s\"",
           read_ret, new_handle, buffer);


    int delete_ret = syscall_delete(new_file);
    assert(delete_ret != 0,
           "File with pathname %s should not be deletable. Return value %d.",
           new_file, delete_ret);

    syscall_close(new_handle);

    /*delete_ret = syscall_delete(new_file);
    assert(delete_ret == 0,
           "File with pathname %s should be deletable if the previous "
           "syscall_create was successful.\nReturn value %d.",
           new_file, delete_ret);*/

    return 0;
}
