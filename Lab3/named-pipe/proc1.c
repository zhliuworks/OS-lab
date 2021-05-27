/*
 * read from pipe and write to file
 */

#include "copyfile.h"

void main() {
    int fd, len;
    char buf[BUFSIZE];
    FILE *out;

    if (!(out = fopen("test.pipe.out", "w"))) {
        fprintf(stderr, "cannot open target file\n");
        exit(1);
    }

    /* `access`: test for access to named pipe */
    if (access(FIFO, F_OK) == -1) {
        /* inaccessible, and `mkfifo`: create a new pipe */
        if (mkfifo(FIFO, 0666) < 0 && errno != EEXIST) {
            fprintf(stderr, "cannot create pipe\n");
            exit(1);
        }
    }

    // open named pipe (read only)
    if ((fd = open(FIFO, O_RDONLY)) == -1) {
        fprintf(stderr, "cannot open pipe file\n");
        exit(1);
    }

    while((len = read(fd, buf, BUFSIZE)) > 0)
        fwrite(buf, sizeof(char), len, out);
    
    close(fd);
}