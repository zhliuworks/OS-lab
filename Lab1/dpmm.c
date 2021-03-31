#include "dpmm.h"


/* initialize memory manager */
Manager *dpmm_init(unsigned size) {
    Manager *manager = (Manager*)malloc(sizeof(Manager));
    manager->maxsize = size;
    manager->head = (node*)malloc(sizeof(node)); 
    manager->head->m_size = size;
    manager->head->m_addr = 0;
    manager->head->next = manager->head;
    manager->head->prev = manager->head;
    manager->next = manager->head;
    return manager;
}


/* free memory manager */
void dpmm_exit(Manager *manager) {
    if(manager->head) {
        node *p = manager->head;
        if(p != p->next) {  // for multiple nodes
            while(p->next != manager->head) {
                p = p->next;
                free(p->prev);
            }
        }
        free(p);
        free(manager);
    }
}


/* allocate memory block with `size` */
char *lmalloc(Manager *manager, unsigned size) {
    if(!manager->next) {
        manager->next = manager->head;
    }
    if(manager->next) {
        node *p = manager->next;  // Next Fit
        if(p == p->next) {  // for single node
            if(p->m_size >= size) {
                char *addr = p->m_addr;
                if(p->m_size == size) {  // equal
                    manager->next = NULL;
                    manager->head = (p == manager->head) ? p->next : manager->head;
                    manager->head = (p == manager->head) ? NULL : manager->head;
                    free(p);
                } else {  // remain
                    p->m_size -= size;
                    p->m_addr += size;
                }
                return addr;
            } else {
                fprintf(stderr, "[Error] System out of memory. Try to allocate %u B but only %u B are available.\n", size, p->m_size);
                return -1;
            }
        } else {  // for multiple nodes
            unsigned maxsize = 0;
            do {
                if(p->m_size >= size) {  // if this node fits
                    char *addr = p->m_addr;
                    manager->next = p->next;
                    if(p->m_size == size) {  // equal
                        p->prev->next = p->next;
                        p->next->prev = p->prev;    
                        manager->next = NULL;
                        manager->head = (p == manager->head) ? p->next : manager->head;
                        manager->head = (p == manager->head) ? NULL : manager->head;
                        free(p);                   
                    } else {  // remain
                        p->m_size -= size;
                        p->m_addr += size;
                    }
                    return addr;
                } else { // search next node                   
                    maxsize = (p->m_size > maxsize) ? p->m_size : maxsize;
                    p = p->next;
                }
            } while(p != manager->next);
            fprintf(stderr, "[Error] System out of memory. Try to allocate %u B but only %u B are available.\n", size, maxsize);
            return -1;
        }
    } else {
        fprintf(stderr, "[Error] System out of memory. Try to allocate %u B but only %u B are available.\n", size, 0);
        return -1;
    }
}


/* free memory block with `size` at `addr` */
bool lfree(Manager *manager, unsigned size, char *addr) {
    // address out of bounds
    if(addr >= manager->maxsize || addr < 0) {  
        fprintf(stderr, "[Error] Cannot access address that is beyond the system memory.\n");
        return false;    
    }

    // non-empty node list
    if(manager->head) {
        node *p = manager->head;

        /* 1: free the node before head */
        if(addr < p->m_addr) {
            // 1-1: insert a new node
            if(addr + size < p->m_addr) {  
                manager->head = (node*)malloc(sizeof(node));
                manager->head->m_size = size;
                manager->head->m_addr = addr;
                manager->head->next = p;
                manager->head->prev = p->prev;
                manager->head->next->prev = manager->head;
                manager->head->prev->next = manager->head;
                return true;
            }
            // 1-2: merge into the neighbor
            else if(addr + size == p->m_addr){  
                p->m_size += size;
                p->m_addr -= size;
                return true;
            }
            // 1-3: error: free free memory
            else {  
                fprintf(stderr, "[Error] Cannot free memory that is already free.\n");
                return false;                
            }
        } 

        /* 2: free the node after tail */
        else if(addr > p->prev->m_addr) {
            if(addr + size > manager->maxsize) {  // error
                fprintf(stderr, "[Error] Cannot free memory that is beyond the system memory.\n");
                return false;                
            }
            // 2-1: error: free free memory
            if(addr < p->prev->m_addr + p->prev->m_size) {  
                fprintf(stderr, "[Error] Cannot free memory that is already free.\n");
                return false;                 
            } 
            // 2-2: merge into the neighbor
            else if(addr == p->prev->m_addr + p->prev->m_size) {  
                p->prev->m_size += size;
                return true;
            }
            // 2-3: insert a new node
            else  { 
                p = (node*)malloc(sizeof(node));
                p->m_size = size;
                p->m_addr = addr;
                p->next = manager->head;
                p->prev = manager->head->prev;
                p->next->prev = p;
                p->prev->next = p;
                return true;
            } 
        }

        /* 3: free the node in between */
        else {
            while(addr >= p->m_addr) {  // search the insertion position
                p = p->next;
            }
            // [[error: free free memory]]
            if(addr < p->prev->m_addr + p->prev->m_size) {  // 3-1: error: free free memory
                fprintf(stderr, "[Error] Cannot free memory that is already free.\n");
                return false;                
            } 
            // [[merge lower, merge both, free free error]]
            else if(addr == p->prev->m_addr + p->prev->m_size) {
                if(addr + size < p->m_addr) {  // 3-2: merge into the lower neighbor
                    p->prev->m_size += size;
                    return true;
                } else if(addr + size == p->m_addr) {  // 3-3: merge with two neighbors
                    p->prev->m_size += (size + p->m_size);
                    p->prev->next = p->next;
                    p->next->prev = p->prev;
                    manager->next = NULL;
                    manager->head = (p == manager->head) ? p->next : manager->head;
                    manager->head = (p == manager->head) ? NULL : manager->head;
                    free(p);
                    return true;
                } else {  // 3-1: error: free free memory
                    fprintf(stderr, "[Error] Cannot free memory that is already free.\n");
                    return false;                     
                }
            }
            // [[insert, merge higher, free free error]]
            else {
                if(addr + size < p->m_addr) {  // 3-4: insert a new node
                    p->prev->next = (node*)malloc(sizeof(node));
                    p->prev->next->m_size = size;
                    p->prev->next->m_addr = addr;
                    p->prev->next->next = p;
                    p->prev->next->prev = p->prev;
                    p->prev = p->prev->next;
                    return true;
                } else if(addr + size == p->m_addr) {  // 3-5: merge the highest neighbor
                    p->m_size += size;
                    p->m_addr -= size;
                    return true;
                } else {  // 3-1: error: free free memory
                    fprintf(stderr, "[Error] Cannot free memory that is already free.\n");
                    return false; 
                }
            }
        }
    }

    // empty node list
     else {  
        manager->head = (node*)malloc(sizeof(node));
        manager->head->m_size = size;
        manager->head->m_addr = addr;
        manager->head->next = manager->head;
        manager->head->prev = manager->head;
        manager->next = manager->head;
        return true;
    }
}


/* display the linked list */
void display(Manager *manager) {
    if(manager->head) {
        node *p = manager->head;
        printf("[%u|%u B]", p->m_addr, p->m_size);
        while(p->next != manager->head) {
            p = p->next;
            printf("-->[%u|%u B]", p->m_addr, p->m_size);
        } 
    } else {
        printf("NULL");
    }
    printf("\n");
}