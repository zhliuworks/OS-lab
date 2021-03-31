#include "dpmm.h"
#define VIRTUAL_SIZE 0x1000  // 4KB

int main(int argc, char *argv[]) {
    Manager *manager = dpmm_init(VIRTUAL_SIZE);
    char path[20] = "./tests/";
    if(argc != 2) {
        fprintf(stderr, "[Error] The parameter must be a file.\n");
        exit(EXIT_FAILURE);
    }
    strcat(path, argv[1]);
    FILE *fp = fopen(path, "r");
    char buf[1023];
    char op;
    unsigned size;
    unsigned addr;
    char *maddr;
    bool fflag;

    display(manager);
    printf("--------------------------------------------------------------------------------------\n");
    while(fgets(buf, 1023, fp)) {
        if(buf[0] == 'm') {
            sscanf(buf, "%c%u", &op, &size);
            maddr = lmalloc(manager, size);
            if(maddr != -1) {
                printf("√ Successfully allocate %u B memory at address [%u].\n", size, maddr);
            } else {
                printf("× Fail to allocate %u B memory.\n", size);
            }
        } else if(buf[0] == 'f') {
            sscanf(buf, "%c%u%u", &op, &size, &addr);
            fflag = lfree(manager, size, addr);
            if(fflag) {
                printf("√ Successfully free %u B memory at address %u.\n", size, addr);
            } else {
                printf("× Fail to free %u B memory at address %u.\n", size, addr);
            }
        }
        display(manager);
        printf("--------------------------------------------------------------------------------------\n");
    }

    dpmm_exit(manager);
    return 0;
}