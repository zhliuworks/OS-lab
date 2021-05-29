#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/wait.h>

void main() {
    int pipedes[2];
    pid_t pid;

    // create pipe
    if (pipe(pipedes) == -1) {
        perror("cannot create pipe\n");
        exit(1);
    }
    
    // create child process 1
    while ((pid = fork()) == -1);
    if (pid == 0) {
        /* child process 1 */ 
        // close file descriptor 1 (stdout)
        if (close(1) == -1) {
            perror("cannot close stdout\n");
            exit(1);
        }
        // duplicate write pipe descriptor and associate with stdout
        if (dup(pipedes[1]) != 1) {
            perror("cannot duplicate write pipe descriptor\n");
            exit(1);
        }
        // close the original pipe descriptors inherited from parent
        if (close(pipedes[0]) == -1 || close(pipedes[1]) == -1) {
            perror("cannot close original pipe descriptors\n");
            exit(1);
        }
        execlp("who", "who", NULL);
    }

    // create child process 2
    while ((pid = fork()) == -1);
    if (pid == 0) {
        /* child process 2 */
        // close file descriptor 0 (stdin)
        if (close(0) == -1) {
            perror("cannot close stdin\n");
            exit(1);
        }
        // duplicate read pipe descriptor and associate with stdin
        if (dup(pipedes[0]) != 0) {
            perror("cannot duplicate read pipe descriptor\n");
            exit(1);
        }
        // close the original pipe descriptors inherited from parent
        if (close(pipedes[0]) == -1 || close(pipedes[1]) == -1) {
            perror("cannot close original pipe descriptor\n");
            exit(1);
        }
        execlp("wc", "wc", "-l", NULL);
    }

    if (close(pipedes[0]) == -1 || close(pipedes[1]) == -1) {
        perror("cannot close pipe descriptors\n");
        exit(1);
    }
    while (wait(NULL) != -1);
}