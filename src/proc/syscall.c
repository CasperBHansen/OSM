/*
 * System calls.
 *
 * Copyright (C) 2003 Juha Aatrokoski, Timo Lilja,
 *   Leena Salmela, Teemu Takanen, Aleksi Virtanen.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: syscall.c,v 1.3 2004/01/13 11:10:05 ttakanen Exp $
 *
 */
#include "drivers/device.h"
#include "drivers/gcd.h"
#include "fs/vfs.h"
#include "kernel/assert.h"
#include "kernel/cswitch.h"
#include "kernel/halt.h"
#include "kernel/interrupt.h"
#include "kernel/panic.h"
#include "kernel/semaphore.h"
#include "lib/libc.h"
#include "proc/process.h"
#include "proc/semaphore.h"
#include "proc/syscall.h"
#include "vm/pagepool.h"
#include "vm/vm.h"

/**
 * Handle SYSCALL_EXIT syscall.
 */
void handle_syscall_exit(context_t * user_context) {
    const int retval = (const int) user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = retval;
    process_id_t pid = process_get_current_process();
    (pid == PROCESS_STARTUP_PID) ? halt_kernel() : process_finish(retval);
}


/**
 * Handle SYSCALL_MEMLIMIT syscall.
 */
void handle_syscall_memlimit(context_t *user_context) {
    const uint32_t heap_end = user_context->cpu_regs[MIPS_REGISTER_A1];
    uint32_t curr_heap = (uint32_t) process_get_current_process_entry()->heap_end;

    if (heap_end == (uint32_t)NULL) {
        user_context->cpu_regs[MIPS_REGISTER_V0] = curr_heap;
        return;
    }

    // we cannot decrease the heap
    if (heap_end < curr_heap) KERNEL_PANIC("Cannot decrease heap; vm_unmap unimplemented.");
    
    // size required by the allocation minus the current pages
    // gives us the amount to be allocated
    int page_size = heap_end / PAGE_SIZE - curr_heap / PAGE_SIZE;
    
    interrupt_status_t intr_status;
    intr_status = _interrupt_disable();
    
    // if we're exactly on a page limit then add a page,
    // otherwise allocate the remainder
    curr_heap += (curr_heap % PAGE_SIZE == 0) ? PAGE_SIZE : (curr_heap % PAGE_SIZE);
    
    // map the pages, shamelessly stolen from process.c
    int i;
    for (i = 0; i < page_size; ++i) {
        uint32_t phys_page = pagepool_get_phys_page();
        KERNEL_ASSERT(phys_page != 0);
        vm_map(thread_get_current_thread_entry()->pagetable, phys_page, curr_heap + i * PAGE_SIZE, 1);
    }
    
    _interrupt_set_state(intr_status);
    
    process_get_current_process_entry()->heap_end = (heap_ptr_t *)heap_end;
    user_context->cpu_regs[MIPS_REGISTER_V0] = heap_end;
}


/**
 * Handle SYSCALL_CLOSE syscall.
 *
 * Effects: Invalidates the file handle passed as parameter. Hereafter the
 *          handle cannot be used in file operations any more.
 *
 * Returns: 0 on success or a negative integer on error.
 */
int handle_syscall_close(int filehandle) {
    if (process_remove_open_file(filehandle) == 0)
        // avoiding std{in,out,err} (see handle_syscall_open)
        return vfs_close(filehandle - 2);

    return -1;
}


/**
 * Handle SYSCALL_DELETE syscall.
 *
 * Effects: Deletes the file referenced by pathname. The operation fails if the
 *          file is open.
 *
 * Returns: 0 on success or a negative integer on error.
 */
int handle_syscall_delete(const char *pathname) {
    return process_is_file_open((char *) pathname) ? -1 : vfs_remove((char *) pathname);
}


/**
 * Handle SYSCALL_OPEN syscall.
 *
 * Effects: Prepares the file referenced by pathname for reading and writing. A
 *          path name includes a volume name and a file name (for instance
 *          [root]a.out).
 *
 * Returns: A positive integer greater than 2 (serving as a file handle to the
 *          file opened) on success or a negative integer on error.
 */
int handle_syscall_open(const char *pathname) {
    // Check if vfs_open returns a valid non-negative file handle, i.e. >= 1,
    // and return this plus 2 if it does to avoid conflicting with stdin (0),
    // stdout (1) and stderr (2).
    openfile_t handle = vfs_open((char *) pathname);
    if (handle < 1)
        return -1;  // error opening file in filesystem

    if (process_add_open_file(handle + 2, (char *) pathname) == 0)  // add to PCB array of open files
        return handle + 2;
    else {
        // no room for another open file in process PCB
        vfs_close(handle);
        return -1;
    }
}


/**
 * Handle SYSCALL_READ syscall.
 *
 * Effects: Reads at most length bytes from the file identified by filehandle
 *          (at the current file position) into the specified buffer, advancing
 *          the file position.
 *
 * Returns: The number of bytes actually read (before reaching the end of the
 *          file) or a negative value when an error occurred.
 */
int handle_syscall_read(int filehandle, void *buffer, int length) {
    if (filehandle == FILEHANDLE_STDIN) {
        device_t *dev = device_get(YAMS_TYPECODE_TTY, 0);
        gcd_t *gcd = (gcd_t *) dev->generic_device;
        int len = gcd->read(gcd, buffer, length);
        return len;
    }
    return vfs_read(filehandle - 2, buffer, length);
}


/**
 * Handle SYSCALL_SEEK syscall.
 *
 * Effects: Sets the reading or writing position of the open file
 *          identified by filehandle to the indicated absolute offset
 *          (in bytes from the beginning).
 *
 * Returns: 0 on success or a negative integer on error. Seeking beyond
 *          the end of the file sets the position to the end and produces
 *          an error return value.
 */
int handle_syscall_seek(int filehandle, int offset) {

    if (!process_is_file_open_in_current_process(filehandle))
        return -1;
    
    if (offset < 0)
        kprintf("Illegal offset!");
    
    char buff;
    vfs_seek(filehandle - 2, 0);

    int i;
    for(i = 0; i <= offset; i++)
        if (! vfs_read(filehandle - 2, &buff, sizeof(char)))
            return -1;

    return 0;
}


/**
 * Handle SYSCALL_WRITE syscall.
 *
 * Effects: Writes length bytes from the specified buffer to the open file
 *          identified by
 *
 * Returns: The number of bytes actually written or a negative integer on
 *          error.
 */
int handle_syscall_write(int filehandle, void *buffer, int length) {
    if (filehandle == FILEHANDLE_STDOUT || filehandle == FILEHANDLE_STDERR) {
        device_t *dev = device_get(YAMS_TYPECODE_TTY, 0);
        gcd_t *gcd = (gcd_t *) dev->generic_device;
        int len = gcd->write(gcd, buffer, (int) length);
        return len;
    }
    return vfs_write(filehandle - 2, buffer, length);
}


/**
 * Handle system calls. Interrupts are enabled when this function is
 * called.
 *
 * @param user_context The userland context (CPU registers as they
 * where when system call instruction was called in userland)
 */
void syscall_handle(context_t *user_context)
{
    int A1 = user_context->cpu_regs[MIPS_REGISTER_A1];
    int A2 = user_context->cpu_regs[MIPS_REGISTER_A2];
    int A3 = user_context->cpu_regs[MIPS_REGISTER_A3];

    // using define since this is going to be assigned values
    #define V0 (user_context->cpu_regs[MIPS_REGISTER_V0])

    /* When a syscall is executed in userland, register a0 contains
     * the number of the syscall. Registers a1, a2 and a3 contain the
     * arguments of the syscall. The userland code expects that after
     * returning from the syscall instruction the return value of the
     * syscall is found in register v0. Before entering this function
     * the userland context has been saved to user_context and after
     * returning from this function the userland context will be
     * restored from user_context.
     */
    switch(user_context->cpu_regs[MIPS_REGISTER_A0]) {
    case SYSCALL_CLOSE:
        V0 = handle_syscall_close(A1);
        break;
    case SYSCALL_CREATE:
        V0 = vfs_create((char *) A1, A2);
        break;
    case SYSCALL_DELETE:
        V0 = handle_syscall_delete((char *) A1);
        break;
    case SYSCALL_EXEC:
        V0 = process_spawn((const char *) A1);
        break;
    case SYSCALL_EXIT:
        handle_syscall_exit(user_context);
        break;
    case SYSCALL_FILE:
        V0 = vfs_file((char *)A1, A2, (char *)A3);
        break;
    case SYSCALL_FILECOUNT:
        V0 = vfs_count((char *) A1);
        break;
    case SYSCALL_HALT:
        halt_kernel();
        break;
    case SYSCALL_JOIN:
        V0 = process_join((const int) A1);
        break;
    case SYSCALL_MEMLIMIT:
        handle_syscall_memlimit(user_context);
        break;
    case SYSCALL_OPEN:
        V0 = handle_syscall_open((char *) A1);
        break;
    case SYSCALL_READ:
        V0 = handle_syscall_read(A1, (void *) A2, A3);
        break;
    case SYSCALL_SEEK:
        V0 = handle_syscall_seek(A1, A2);
        break;
    case SYSCALL_SEM_DESTROY:
        V0 = semaphore_userland_destroy((usr_sem_t *) A1);
        break;
    case SYSCALL_SEM_OPEN:
        V0 = (uint32_t) semaphore_userland_open((char *) A1, (int) A2);
        break;
    case SYSCALL_SEM_PROCURE:
        V0 = semaphore_userland_procure((usr_sem_t *) A1);
        break;
    case SYSCALL_SEM_VACATE:
        V0 = semaphore_userland_vacate((usr_sem_t *) A1);
        break;
    case SYSCALL_WRITE:
        V0 = handle_syscall_write(A1, (void *) A2, A3);
        break;
    default:
        kprintf("%d", user_context->cpu_regs[MIPS_REGISTER_A0]);
        KERNEL_PANIC("Unhandled system call\n");
    }

    /* Move to next instruction after system call */
    user_context->pc += 4;
}
