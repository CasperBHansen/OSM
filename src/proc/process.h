/*
 * Process startup.
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
 * $Id: process.h,v 1.4 2003/05/16 10:13:55 ttakanen Exp $
 *
 */

#ifndef BUENOS_PROC_PROCESS
#define BUENOS_PROC_PROCESS

#include "kernel/cswitch.h"

typedef int process_id_t;
typedef void heap_ptr_t;

#define USERLAND_STACK_TOP 0x7fffeffc

#define PROCESS_PTABLE_FULL  -1
#define PROCESS_ILLEGAL_JOIN -2

#define PROCESS_MAX_PROCESSES 32
#define PROCESS_STARTUP_PID 0

#define PROCESS_NAME_LENGTH 128

/* Enumeration type of process states. */
typedef enum {
    PROCESS_NEW,
    PROCESS_WAITING,
    PROCESS_RUNNING,
    PROCESS_ZOMBIE,
    PROCESS_DEAD
} process_state_t;

/* Process control block data structure. */
typedef struct {
    /* name of executable */
    char executable[PROCESS_NAME_LENGTH];

    // parent id
    process_id_t parent_id;
    
    // thread id
    int thread_id;

    /* process state */
    process_state_t state;

    // return value
    int retval;

    // heap pointer
    heap_ptr_t *heap_end;
} process_control_block_t;


void process_start(uint32_t pid);

/* Initialize the process table.  This must be called during kernel
   startup before any other process-related calls. */
void process_init();

/* Run process in a new thread. Returns the PID of the new process. */
process_id_t process_spawn(const char *executable);

/* Stop the process and the thread it runs in. Sets the return value as well */
void process_finish(int retval);

/* Wait for the given process to terminate, returning its return value. This
 * will also mark its process table entry as free.
 * Only works on child processes */
int process_join(process_id_t pid);

process_id_t process_create_process(const char * executable);

/* Return PID of current process. */
process_id_t process_get_current_process(void);

/* Return PCB of current process. */
process_control_block_t *process_get_current_process_entry(void);

/* Return PCB with the pid */
process_control_block_t *process_get_process_entry(process_id_t pid);

/* Return an available pid. */
process_id_t process_get_available_pid();

#endif
