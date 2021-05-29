#include "copyfile.h"


/* file copy based on `read()` and `write()` system call */
void copy(char *indir, char *outdir) {
    int in, out, len;
    char buf[BUFSIZE];

    if ((in = open(indir, O_RDONLY)) == -1) {
        fprintf(stderr, "copy: cannot open source file\n");
        exit(1);
    }
    if ((out = open(outdir, O_WRONLY|O_CREAT)) == -1) {
        fprintf(stderr, "copy: cannot open target file\n");
        exit(1);        
    }

	while ((len = read(in, buf, BUFSIZE)) > 0)
	    write(out, buf, len);

//    while ((len = read(in, buf, 1)) > 0)
//        write(out, buf, 1);   

    close(in);
    close(out);
}


/* file copy based on `fread()` and `fwrite()` api */
void fcopy(char *indir, char *outdir) {
    FILE *in, *out;
    int len;
    char buf[BUFSIZE];

    if (!(in = fopen(indir, "r"))) {
        fprintf(stderr, "fcopy: cannot open source file\n");
        exit(1);        
    }
    if (!(out = fopen(outdir, "w"))) {
        fprintf(stderr, "fcopy: cannot open target file\n");
        exit(1);          
    }

    while ((len = fread(buf, sizeof(char), BUFSIZE, in)) > 0)
        fwrite(buf, sizeof(char), len, out);

//    while ((len = fread(buf, 1, 1, in)) > 0)
//        fwrite(buf, 1, 1, out);

    fclose(in);
    fclose(out);
}


/* file copy based on `fscanf()` and `fprintf()` api */
void fcopyf(char *indir, char *outdir) {
    FILE *in, *out;
    char data;

    if (!(in = fopen(indir, "r"))) {
        fprintf(stderr, "fcopyf: cannot open source file\n");
        exit(1);        
    }
    if (!(out = fopen(outdir, "w"))) {
        fprintf(stderr, "fcopyf: cannot open target file\n");
        exit(1);          
    }

    while (fscanf(in, "%c", &data) != EOF)
        fprintf(out, "%c", data);

    fclose(in);
    fclose(out);
}


/* file copy based on `fgetc()` and `fputc()` api */
void fcopyc(char *indir, char *outdir) {
    FILE *in, *out;
    char data;

    if (!(in = fopen(indir, "r"))) {
        fprintf(stderr, "fcopyc: cannot open source file\n");
        exit(1);        
    }
    if (!(out = fopen(outdir, "w"))) {
        fprintf(stderr, "fcopyc: cannot open target file\n");
        exit(1);          
    }

    while ((data = fgetc(in)) != EOF)
        fputc(data, out);

    fclose(in);
    fclose(out);
}


/* file copy based on `fgets()` and `fputs()` api */
void fcopys(char *indir, char *outdir) {
    FILE *in, *out;
    char buf[BUFSIZE];

    if (!(in = fopen(indir, "r"))) {
        fprintf(stderr, "fcopys: cannot open source file\n");
        exit(1);        
    }
    if (!(out = fopen(outdir, "w"))) {
        fprintf(stderr, "fcopys: cannot open target file\n");
        exit(1);          
    }

    while (fgets(buf, BUFSIZE, in))
        fputs(buf, out);

    fclose(in);
    fclose(out);
}


/* main program */
void main(int argc, char *argv[]) {
    if (argc == 2)
        switch (atoi(argv[1]))
        {
        case 0:
            copy("test/test.in", "test/test.copy.out");
            break;
        case 1:
            fcopy("test/test.in", "test/test.fcopy.out");
            break;
        case 2:
            fcopyf("test/test.in", "test/test.fcopyf.out");
            break;
        case 3:
            fcopyc("test/test.in", "test/test.fcopyc.out");
            break;
        case 4:
            fcopys("test/test.in", "test/test.fcopys.out");
            break;
        default:
            break;
        }
}
