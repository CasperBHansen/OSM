/*
 * semaphores.c
 *
 * Test case of userland semaphores.
 */

#include "tests/lib.h"
#include "proc/semaphore.h"

const char str[] = "Hello Semaphores!\n";

const char fail_open_msg[] = "Couldn't open semaphore.\n";
const char fail_destroy[] = "Couldn't destroy semaphore, still occupied.\n";
const char succ_destroy[] = "Destroyed semaphore succesfully.\n";
const char fail_max_sems[] = "Semaphores maxed out, couldn't open semaphore.\n";
const char fail_expect[] = "We will try to max out the possible amount of semaphores, expecting two error messages...\n";

usr_sem_t * sems[10];

int main() {
    
    syscall_write(stdout, str, sizeof(str));
    
    int i;
    char name[] = "semaphore_0";
    for (i = 0; i < 10; ++i) {
        sems[i] = syscall_sem_open(name, 4);
        name[10]++;
    }
    
    // procure to the semaphore's max resources
    for (i = 0; i < 4; ++i) {
        syscall_sem_p(sems[0]);
    }
    
    // vacate once and try to destroy
    syscall_sem_v(sems[0]);
    if (syscall_sem_destroy(sems[0]) != 0) {
        syscall_write(stdout, fail_destroy, sizeof(fail_destroy));
        for (i = 0; i < 3; ++i) {
            syscall_sem_v(sems[0]);
        }
    }
    
    // try to destroy again, should work.
    if (syscall_sem_destroy(sems[0]) != 0)
        syscall_write(stdout, fail_destroy, sizeof(fail_destroy));
    else
        syscall_write(stdout, succ_destroy, sizeof(succ_destroy));
    
    // there are currently 9 semaphores in play, try to go beyond max by two ..
    syscall_write(stdout, fail_expect, sizeof(fail_expect));
    for (i = 0; i < SEMAPHORE_MAX_USER_SEMAPHORES - 7 ; ++i) {
        if ( syscall_sem_open(name, 4) == NULL ) {
            syscall_write(stdout, fail_max_sems, sizeof(fail_max_sems));
        }
            
        name[10]++;
    }
    
    return 0;
}
