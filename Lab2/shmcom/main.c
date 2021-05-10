/*
 *  main.c  --  shared memory communication main program
 */

#include "shmcom.h"

void main() {
    pid_t pid;
    int status;
    char *segaddr; // shared memory entry address
    int sid;  // shared memory id
    sem_t empty, full;

    // get shared memory segment
    if ((sid = shmget(SHMCOM_KEY, SHM_SIZE, IPC_CREAT|0666)) == -1) {
        perror("shmget error\n");
    }
    // attach shared memory segment
    segaddr = shmat(sid, 0, 0);
    // create two semaphores (init val = 1)
    empty = createSem(SEM_EMPTY_KEY);
    full = createSem(SEM_FULL_KEY);
    // set `full` = 0 (init empty buffer)
    semWait(full); 

    // communication between parent and child process
    while ((pid = fork()) == -1);
    if (pid) {
        /* parent process: read and store */
        char buffer_p[BUFFER_SIZE];
        FILE *in = fopen("../test/test.in", "r");

        if (fread(buffer_p, sizeof(char), BUFFER_SIZE, in) > 0) {
            semWait(empty);
            strcpy(segaddr, buffer_p); 
            semSignal(full);
        }

        if (wait(&status) == -1) {
            perror("wait error\n");
        }

        fclose(in);
    } 
    else {
        /* child process: load and write */
        char buffer_c[BUFFER_SIZE];
        int length;
        FILE *out = fopen("../test/test.shm.out", "w");

        semWait(full);
        length = strlen(segaddr);
        strcpy(buffer_c, segaddr);
        semSignal(empty);

        fwrite(buffer_c, sizeof(char), length, out);

        fclose(out);
        exit(0);
    }

    exit(0);
}