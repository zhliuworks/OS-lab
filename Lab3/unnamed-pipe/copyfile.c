#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 1024

void main() {
    /*
     * pipe file descriptor:
     *   fd_pipe[0]: read
     *   fd_pipe[1]: write
     */
    int fd_pipe[2];
    pid_t pid;
    char buf[BUFSIZE];
    int len;
    FILE *in, *out;

    if (!(in = fopen("test.in", "r"))) {
        fprintf(stderr, "cannot open source file\n");
        exit(1);        
    }
    if (!(out = fopen("test.upipe.out", "w"))) {
        fprintf(stderr, "cannot open target file\n");
        exit(1);          
    }

    /* create a one-way communication pipe */
    if (pipe(fd_pipe) == -1) {
        fprintf(stderr, "cannot create pipe\n");
    }

    if (pid = fork()) {
        /* 
         * parent process:
         *   read from file and write to pipe
         */
        close(fd_pipe[0]);
        while ((len = fread(buf, sizeof(char), BUFSIZE, in)) > 0)
            write(fd_pipe[1], buf, len);
        close(fd_pipe[1]);
    } else {
        /* 
         * child process:
         *   read from pipe and write to file
         */
        close(fd_pipe[1]);
        while ((len = read(fd_pipe[0], buf, BUFSIZE)) > 0)
            fwrite(buf, sizeof(char), len, out);
        close(fd_pipe[0]);
    }
}