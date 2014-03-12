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
 * Handle SYSCALL_EXEC syscall.
 */
void handle_syscall_exec(context_t * user_context) {
    const char * executable = (const char *)user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = process_spawn(executable);
}

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
 * Handle SYSCALL_JOIN syscall.
 */
void handle_syscall_join(context_t * user_context) {
    const uint32_t pid = (const int) user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = process_join(pid);
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
 * Handle SYSCALL_READ syscall.
 */
void handle_syscall_read(context_t *user_context) {
    uint32_t fhandle = user_context->cpu_regs[MIPS_REGISTER_A1];
    uint8_t *buffer  = (uint8_t *) user_context->cpu_regs[MIPS_REGISTER_A2];
    int length  = user_context->cpu_regs[MIPS_REGISTER_A3];

    if (fhandle == FILEHANDLE_STDIN) {
        device_t *dev = device_get(YAMS_TYPECODE_TTY, 0);
        gcd_t *gcd = (gcd_t *) dev->generic_device;
        int len = gcd->read(gcd, buffer, length);
        // buffer[len] = '\0';
        user_context->cpu_regs[MIPS_REGISTER_V0] = len;
    } else {
        // error; reading from other files unimplemented
        user_context->cpu_regs[MIPS_REGISTER_V0] = -1;
    }
}

/**
 * Handle SYSCALL_WRITE syscall.
 */
void handle_syscall_write(context_t *user_context) {
    uint32_t fhandle = user_context->cpu_regs[MIPS_REGISTER_A1];
    uint8_t *buffer  = (uint8_t *) user_context->cpu_regs[MIPS_REGISTER_A2];
    uint32_t length  = user_context->cpu_regs[MIPS_REGISTER_A3];

    if (fhandle == FILEHANDLE_STDOUT) {
        device_t *dev = device_get(YAMS_TYPECODE_TTY, 0);
        gcd_t *gcd = (gcd_t *) dev->generic_device;
        int len = gcd->write(gcd, buffer, (int) length);
        // null terminate, TODO: avoid overflow
        //buffer[len] = '\0';
        user_context->cpu_regs[MIPS_REGISTER_V0] = len;
    } else {
        user_context->cpu_regs[MIPS_REGISTER_V0] = -1;
    }
}

/*
 * Handle SYSCALL_SEM_DESTROY syscall.
 */
void handle_syscall_sem_destroy(context_t * user_context) {
    usr_sem_t * sem = (usr_sem_t *)user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = (uint32_t)semaphore_userland_destroy(sem);
}

/*
 * Handle SYSCALL_SEM_OPEN syscall.
 */
void handle_syscall_sem_open(context_t * user_context) {
    const char * name = (const char *)user_context->cpu_regs[MIPS_REGISTER_A1];
    const int value = (const int)user_context->cpu_regs[MIPS_REGISTER_A2];
    user_context->cpu_regs[MIPS_REGISTER_V0] = (uint32_t)semaphore_userland_open(name, value);
}

/*
 * Handle SYSCALL_SEM_PROCURE syscall.
 */
void handle_syscall_sem_procure(context_t * user_context) {
    usr_sem_t * sem = (usr_sem_t *)user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = (uint32_t)semaphore_userland_procure(sem);
}

/*
 * Handle SYSCALL_SEM_VACATE syscall.
 */
void handle_syscall_sem_vacate(context_t * user_context) {
    usr_sem_t * sem = (usr_sem_t *)user_context->cpu_regs[MIPS_REGISTER_A1];
    user_context->cpu_regs[MIPS_REGISTER_V0] = (uint32_t)semaphore_userland_vacate(sem);
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
                                    // TODO: may be redundant
    if (filehandle < 2) return -1;  // not a valid closeable filehandle
    else return vfs_close(filehandle - 2); // again avoiding std{in,out,err}
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
    // Check pathname is not too long, as defined in fs/vfs.h.
    if (strlen(pathname) > VFS_PATH_LENGTH) {
        kprintf("Filepath exceeds the maximum length of %d.\n\
                 The file %s was not opened.\n", VFS_PATH_LENGTH, pathname);
        return -1;
    }

    // Check if vfs_open returns a valid non-negative filehandle, i.e. >= 1,
    // and return this plus 2 if it does to avoid conflicting with stdin (0),
    // stdout (1) and stderr (2).
    openfile_t handle = vfs_open((char *) pathname);
    if (handle < 1)
        return -1;  // error opening file in filesystem
    else
        return handle + 2;
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
    /*int A2 = user_context->cpu_regs[MIPS_REGISTER_A2];
    int A3 = user_context->cpu_regs[MIPS_REGISTER_A3];*/

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
    case SYSCALL_EXEC:
        handle_syscall_exec(user_context);
        break;
    case SYSCALL_EXIT:
        handle_syscall_exit(user_context);
        break;
    case SYSCALL_HALT:
        halt_kernel();
        break;
    case SYSCALL_JOIN:
        handle_syscall_join(user_context);
        break;
    case SYSCALL_MEMLIMIT:
        handle_syscall_memlimit(user_context);
        break;
    case SYSCALL_OPEN:
        V0 = handle_syscall_open((char *) A1);
        break;
    case SYSCALL_READ:
        handle_syscall_read(user_context);
        break;
    case SYSCALL_SEM_DESTROY:
        handle_syscall_sem_destroy(user_context);
        break;
    case SYSCALL_SEM_OPEN:
        handle_syscall_sem_open(user_context);
        break;
    case SYSCALL_SEM_PROCURE:
        handle_syscall_sem_procure(user_context);
        break;
    case SYSCALL_SEM_VACATE:
        handle_syscall_sem_vacate(user_context);
        break;
    case SYSCALL_WRITE:
        handle_syscall_write(user_context);
        break;
    default:
        kprintf("%d", user_context->cpu_regs[MIPS_REGISTER_A0]);
        KERNEL_PANIC("Unhandled system call\n");
    }

    /* Move to next instruction after system call */
    user_context->pc += 4;
}
