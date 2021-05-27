#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 1024

/* file copy based on `read()` and `write()` system call */
void copy(char *indir, char *outdir);

/* file copy based on `fread()` and `fwrite()` api */
void fcopy(char *indir, char *outdir);

/* file copy based on `fscanf()` and `fprintf()` api */
void fcopyf(char *indir, char *outdir);

/* file copy based on `fgetc()` and `fputc()` api */
void fcopyc(char *indir, char *outdir);

/* file copy based on `fgets()` and `fputs()` api */
void fcopys(char *indir, char *outdir);