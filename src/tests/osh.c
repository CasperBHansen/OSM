/*
 * OSM shell.
 */

#include "tests/lib.h"

#define BUFFER_SIZE 100
#define PATH_LENGTH 256


typedef int (*cmd_fun_t)(int, char**);

typedef struct {
    char* name;
    cmd_fun_t func;
    char *desc;
} cmd_t;

int cmd_echo(int, char**);
int cmd_show(int, char**);
int cmd_read(int, char**);
int cmd_rm(int, char**);
int cmd_exit();
int cmd_ls(int, char**);
int cmd_cp(int, char**);
int cmd_cmp(int, char**);
int cmd_help();

cmd_t commands[] =
{
//    {"cmp",  cmd_cmp,  "removes a file"},
    {"cp",   cmd_cp,   "removes a file"},
    {"echo", cmd_echo, "print the arguments to the screen"},
    {"exit", cmd_exit, "removes a file"},
    {"help", cmd_help, "show this help message"},
    {"ls",   cmd_ls,   "removes a file"},
    {"read", cmd_read, "read a line from standard in and write it to a new file"},
    {"rm",   cmd_rm,   "removes a file"},
    {"show", cmd_show, "print the contents of the given file to the screen"}
};

#define N_COMMANDS sizeof(commands) / sizeof(cmd_t)

void print_prompt(int last_retval) {
    printf("%d> ", last_retval);
}

/* Note that tokenize(cmdline, argv) modifies cmdline by inserting NUL
   characters. */
int tokenize(char* cmdline, char** argv) {
    int argc = 0;
    int inword=0;
    char *s, *p;
    for (s = cmdline, p = cmdline; *s; s++) {
        if (*s == ' ' && inword) {
            inword=0;
            argv[argc++]=p;
            *s = '\0';
        } else if (*s != ' ' && !inword) {
            inword=1;
            p=s;
        }
    }
    if (inword) {
        argv[argc++]=p;
    }
    return argc;
}

int run_program(char* prog) {
    return syscall_join(syscall_exec(prog));
}

int run_command(char* cmdline) {
    char* argv[BUFFER_SIZE];
    int argc = tokenize(cmdline, argv);
    unsigned int i;
    if (argc == 0) {
        return 0;
    }
    for (i = 0; i < N_COMMANDS; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            return commands[i].func(argc, argv);
        }
    }
    return run_program(cmdline);
}

void help() {
    printf("Welcome to the Buenos Shell!\n");
    printf("The following commands are available:\n");
    unsigned int i;
    for (i = 0; i < N_COMMANDS; i++) {
        printf("  %s: %s\n", commands[i].name, commands[i].desc);
    }
}

int main(void) {
    char cmdline[BUFFER_SIZE];
    int ret = 0;
    help();
    while (1) {
        print_prompt(ret);
        readline(cmdline, BUFFER_SIZE);
        run_command(cmdline);
    }
    syscall_halt();
    return 0;
}

int cmd_cp(int argc, char** argv)
{
    if (argc != 3) {
        printf("Usage: cp <source> <destination>\n");
        return -1;
    }
    
    int src = syscall_open(argv[1]);
    if (src < 3)
        return -1;
    
    int size = 0;
    char dummy;
    int bah;
    while ( (bah = syscall_read(src, &dummy, 1)) != 0) {
        printf("dummy: %i, bah = %i\n", dummy, bah);
        ++size;
    }
    
    char * buffer = (char *)malloc(sizeof(char) * size);
    
    syscall_seek(src, 0);
    int bytes_read = syscall_read(src, buffer, size);
    if (bytes_read != size)
        return -1;
    
    
    if (syscall_create(argv[2], bytes_read) != 0)
        if (syscall_delete(argv[2]) != 0 ||
            syscall_create(argv[2], bytes_read) != 0)
            return -1;
    
    int dst = syscall_open(argv[2]);
    if (syscall_write(dst, buffer, size) != size)
        return -1;
    
    return 0;
}

int cmd_exit()
{
    syscall_halt();
    return 0;
}

int cmd_ls(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: ls <volumename>\n");
        return -1;
    }
    
    // TODO: test for fragments?
    char buffer[256];
    int i;
    for (i = 0; i < syscall_filecount(argv[1]); ++i)
        if (syscall_file(argv[1], i, &buffer[0]) == 0)
            printf("%s\n", buffer);
    
    return 0;
}

int cmd_rm(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: rm <file> ...\n");
        return -1;
    }

    int i = 1;
    int rc;
    while (i < argc) {
        if ((rc = syscall_delete(argv[i])) != 0)
            printf("%s: cannot remove '%s': No such file\n", argv[0], argv[i]);
        i++;
    }
    
    if (i == argc) return 1;
    else return 0;
}

int cmd_echo(int argc, char** argv) {
    int i;
    for (i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    puts("\n");
    return 0;
}

int cmd_show(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: show <file>\n");
        return 1;
    }
    int fd;
    if ((fd=syscall_open(argv[1])) < 0) {
        printf("Could not open %s.  Reason: %d\n", argv[1], fd);
        return 1;
    }

    int rd;
    char buffer[BUFFER_SIZE];
    while ((rd = syscall_read(fd, buffer, BUFFER_SIZE))) {
        int wr=0, thiswr;
        while (wr < rd) {
            if ((thiswr = syscall_write(1, buffer+wr, rd-wr)) <= 0) {
                printf("\nCall to syscall_write() failed.  Reason: %d.\n", wr);
                syscall_close(fd);
                return 1;
            }
            wr += thiswr;
        }
    }
    if (rd < 0) {
        printf("\nCall to syscall_read() failed.  Reason: %d.\n", rd);
        syscall_close(fd);
        return 1;
    } else {
        syscall_close(fd);
        return 0;
    }
}

int cmd_read(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: read <file>\n");
        return 1;
    }
    char text[BUFFER_SIZE];
    int count, ret, fd, wr;
    count = readline(text, BUFFER_SIZE - 1) + 1;
    text[count - 1] = '\n';
    text[count] = '\0';
    if ((ret=syscall_create(argv[1], count)) < 0) {
        printf("Could not create %s with initial size %d.  Reason: %d\n", argv[1], count, ret);
        return 1;
    }
    if ((fd=syscall_open(argv[1])) < 0) {
        printf("Could not open %s.  Reason: %d\n", argv[1], fd);
        return 1;
    }
    if ((wr=syscall_write(fd, text, count)) <= 0) {
        printf("\nCall to syscall_write() failed.  Reason: %d.\n", wr);
        syscall_close(fd);
        return 1;
    }
    syscall_close(fd);
    return 0;
}

int cmd_help() {
    help();
    return 0;
}
