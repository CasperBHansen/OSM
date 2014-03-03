/*
 * semaphore.h
 */

#ifndef BUENOS_USERLAND_SEMAPHORE_H
#define BUENOS_USERLAND_SEMAPHORE_H

#include "kernel/semaphore.h"
#include "kernel/config.h"

#define SEMAPHORE_NAME_LENGTH 128
#define SEMAPHORE_MAX_USER_SEMAPHORES (CONFIG_MAX_SEMAPHORES / 2)

typedef void usr_sem_t;

void semaphore_userland_init(void);

usr_sem_t * semaphore_userland_open(const char * name, int value);
int semaphore_userland_procure(usr_sem_t * handle);
int semaphore_userland_vacate(usr_sem_t * handle);
int semaphore_userland_destroy(usr_sem_t * handle);

#endif /* BUENOS_USERLAND_SEMAPHORE_H */

