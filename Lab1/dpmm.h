#ifndef __DYNAMIC_PARTITIONING_MEMORY_MANAGEMENT__
#define __DYNAMIC_PARTITIONING_MEMORY_MANAGEMENT__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* partitioning table organized as doubly linked list */
typedef struct map {
    unsigned m_size;
    char *m_addr;
    struct map *next, *prev;
} node;

typedef struct {
    node *head, *next;
    unsigned maxsize;
} Manager;

/* initialize memory manager */
Manager *dpmm_init(unsigned size);

/* free memory manager */
void dpmm_exit(Manager *manager);

/* allocate memory block with `size` */
char *lmalloc(Manager *manager, unsigned size);

/* free memory block with `size` at `addr` */
bool lfree(Manager *manager, unsigned size, char *addr);

/* display the linked list */
void display(Manager *manager);

#endif