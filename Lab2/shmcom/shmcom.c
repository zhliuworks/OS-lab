/*
 *  shmcom.c  --  shared memory communication source file
 */

#include "shmcom.h"

/* create semaphore */
sem_t createSem(key_t key) {
    sem_t sem;
    union semun {
        int val;
        struct semid_ds *buf;
        __u_short *array;
    } arg;
    // get semaphore
    if ((sem = semget(key, 1, IPC_CREAT|0666)) == -1) {
        perror("semget error\n");
    }
    // control: `SETVAL` initialize semaphore value with `arg.val = 1`
    arg.val = 1;
    if (semctl(sem, 0, SETVAL, arg) == -1) {
        perror("semctl error\n");
    }
    return sem;
}

/* semaphore operation wrapper function */
static void semCall(sem_t sem, int op) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;
    sb.sem_flg = 0;
    // operate on semaphore
    if (semop(sem, &sb, 1) == -1) {
        perror("semop error\n");
    }
}

/* semaphore wait function */
void semWait(sem_t sem) {
    semCall(sem, -1);
}

/* semaphore signal function */
void semSignal(sem_t sem) {
    semCall(sem, 1);
}