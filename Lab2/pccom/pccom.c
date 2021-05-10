/*
 *  pccom.c  --  parent-child communication with system calls
 * 
 *  1. create child process with `fork()`
 *  2. make process sleep with `sleep()`
 *  3. change process image with `execv()`
 *  4. wait for child process to terminate with `wait()/waitpid()`
 *  5. set interrupt handler with `signal()`
 *  6. send signal with `kill()`
 *  7. terminate process with `exit()`
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void sighandler() {
    printf("[Signal] Start signal handler of SIGUSR1\n");
}

void main() {
    pid_t pid;
    int status;
    signal(SIGUSR1, sighandler);

    while ((pid = fork()) == -1);
    if (pid) {
        /* parent process */
        printf("[Parent] Start parent process: %d\n", getpid());
        sleep(2);  // sleeping
        printf("[Parent] Send signal...\n");
        kill(pid, SIGUSR1);  // send a SIGUSR1 signal to child process
        printf("[Parent] Wait for child to end...\n");
        if (waitpid(pid, &status, 0) < 0) {  // wait for child process to terminate
            printf("[Parent] waitpid error\n");
        } else {
            if (WIFEXITED(status))
                printf("[Parent] Child process exited with status: %d\n", WEXITSTATUS(status));
        }
        printf("[Parent] Parent process ended\n");
        exit(0);
    } else {
        /* child process */
        printf("[Child] Start child process: %d\n", getpid());
        pause();  // sleep until received a signal
        char *argv[] = {"uname", "-a", (char*)0};
        sleep(2);  // sleeping
        printf("[Child] Start executing `uname -a`...\n");
        execv("/bin/uname", argv);  // execute a built-in program
        printf("[Child] execv error\n");
        exit(1);  // exit abnormally
    }
}