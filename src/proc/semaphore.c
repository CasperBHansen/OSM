/*
 * semaphore.c
 */

#include "proc/semaphore.h"

#include "lib/libc.h"
#include "kernel/interrupt.h"
#include "kernel/spinlock.h"

typedef struct {
    const char name[SEMAPHORE_NAME_LENGTH];
    semaphore_t * kernel_sem;
    int resources;
} sem_t;

static sem_t semaphore_userland_table[SEMAPHORE_MAX_USER_SEMAPHORES];
static spinlock_t semaphore_userland_table_slock;

// helper function, not exposed to the user
usr_sem_t * semaphore_userland_get(const char * name)
{
    int i = 0;
    for (i = 0; i < SEMAPHORE_MAX_USER_SEMAPHORES; ++i)
        if (semaphore_userland_table[i].kernel_sem &&
            stringcmp(name, semaphore_userland_table[i].name) == 0)
            return (usr_sem_t *)&semaphore_userland_table[i];
    
    return NULL;
}

void semaphore_userland_init(void)
{
    spinlock_reset(&semaphore_userland_table_slock);
    
    interrupt_status_t intr_status;
    
    intr_status = _interrupt_disable();
    spinlock_acquire(&semaphore_userland_table_slock);
    
    int i;
    for (i = 0; i < SEMAPHORE_MAX_USER_SEMAPHORES; ++i) {
        semaphore_userland_table[i].kernel_sem = NULL;
        semaphore_userland_table[i].resources = -1;
    }
    
    spinlock_release(&semaphore_userland_table_slock);
    _interrupt_set_state(intr_status);
}

usr_sem_t * semaphore_userland_open(const char * name, int value)
{
    sem_t * sem = (sem_t *)semaphore_userland_get(name);
    
    if (value < 0) return sem; // fetch a pre-existing semaphore
    if (sem) return NULL; // error, semaphore already exists
        
    interrupt_status_t intr_status;
    
    intr_status = _interrupt_disable();
    spinlock_acquire(&semaphore_userland_table_slock);
    
    static int next = 0; 
    int sem_id, i;
    // find available space for a new semaphore
    for(i = 0; i < SEMAPHORE_MAX_USER_SEMAPHORES; i++) {
        sem_id = next;
        next = (next + 1) % SEMAPHORE_MAX_USER_SEMAPHORES;
        if (semaphore_userland_table[sem_id].kernel_sem == NULL) {
            // space found, create the semaphore
            stringcopy((char *)semaphore_userland_table[sem_id].name, name, SEMAPHORE_NAME_LENGTH);
            semaphore_userland_table[sem_id].kernel_sem = semaphore_create(value);
            semaphore_userland_table[sem_id].resources = value;
            // kprintf("Created semaphore with id %i\n", sem_id);
            break;
        }
    }
    
    spinlock_release(&semaphore_userland_table_slock);
    _interrupt_set_state(intr_status);
    
    if (i == SEMAPHORE_MAX_USER_SEMAPHORES) return NULL;
    
    return (usr_sem_t *)&semaphore_userland_table[sem_id];
}

int semaphore_userland_procure(usr_sem_t * handle)
{
    int ret = 0;
    interrupt_status_t intr_status;
    
    sem_t * sem = (sem_t *)semaphore_userland_get(handle);
    
    intr_status = _interrupt_disable();
    spinlock_acquire(&semaphore_userland_table_slock);
    
    if (sem->kernel_sem) semaphore_P(sem->kernel_sem);
    else ret = -1;
    
    spinlock_release(&semaphore_userland_table_slock);
    _interrupt_set_state(intr_status);
    
    return ret;
}

int semaphore_userland_vacate(usr_sem_t * handle)
{
    int ret = 0;
    interrupt_status_t intr_status;
    
    sem_t * sem = (sem_t *)semaphore_userland_get(handle);
    
    intr_status = _interrupt_disable();
    spinlock_acquire(&semaphore_userland_table_slock);
    
    if (sem->kernel_sem) semaphore_V(sem->kernel_sem);
    else ret = -1;
    
    spinlock_release(&semaphore_userland_table_slock);
    _interrupt_set_state(intr_status);
    
    return ret;
}

int semaphore_userland_destroy(usr_sem_t * handle)
{
    int ret = 0;
    interrupt_status_t intr_status;
    
    sem_t * sem = (sem_t *)semaphore_userland_get(handle);
    
    intr_status = _interrupt_disable();
    spinlock_acquire(&semaphore_userland_table_slock);
    
    // alternatively, to give more info return the difference
    if (sem->kernel_sem->value != sem->resources) ret = -1;
    else {
        // destroy the kernel semaphore, and invalidate the userland semaphore.
        semaphore_destroy(sem->kernel_sem);
        sem->kernel_sem = NULL;
    }
    
    spinlock_release(&semaphore_userland_table_slock);
    _interrupt_set_state(intr_status);
    
    return ret;
}

