/*
 *  shmcom.h  --  shared memory communication header file
 */

#ifndef __SHARED_MEMORY_COMM__
#define __SHARED_MEMORY_COMM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHMCOM_KEY 123456789
#define SHM_SIZE 1024
#define SEM_EMPTY_KEY 13579
#define SEM_FULL_KEY 24680
#define BUFFER_SIZE 1024

typedef int sem_t;

/* create semaphore */
sem_t createSem(key_t key);

/* semaphore wait function */
void semWait(sem_t sem);

/* semaphore signal function */
void semSignal(sem_t sem);

#endif