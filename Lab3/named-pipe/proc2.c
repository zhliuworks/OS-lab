/*
 * read from file and write to pipe
 */

#include "copyfile.h"

void main() {
    int fd, len;
    char buf[BUFSIZE];
    FILE *in;

    if (!(in = fopen("test.in", "r"))) {
        fprintf(stderr, "cannot open source file\n");
        exit(1);
    } 

    // open named pipe (write only)
    if ((fd = open(FIFO, O_WRONLY)) == -1) {
        fprintf(stderr, "cannot open pipe file\n");
        exit(1);
    }

    while((len = fread(buf, sizeof(char), BUFSIZE, in)) > 0)
        write(fd, buf, len);

    close(fd);
}