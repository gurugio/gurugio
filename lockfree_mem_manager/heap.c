
#include <stdlib.h>

#include "heap.h"




inline void make_active(struct active_struct *act_struct, active_t *act)
{
    *act = (active_t)((__ull(act_struct->addr) & ACTIVE_ADDR_MASK) |    \
                      (act_struct->credits & ACTIVE_CREDITS_MASK));
    
}

inline void extract_active(struct active_struct *act_struct,
                    active_t *act)
{
    act_struct->addr = (void *)ACTIVE_GET_ADDR(*act);
    act_struct->credits = ACTIVE_GET_CREDITS(*act);
}
    


procheap_t *alloc_heap_desc(void)
{
    procheap_t *new_heaps;
    
    // create heaps
    new_heaps = (procheap_t *)malloc(sizeof(procheap_t));

    if (new_heaps == NULL)
        return NULL;

    // set active
    ACTIVE_SET_ADDR(new_heaps->active, 0);
    ACTIVE_SET_CREDITS(new_heaps->active, 0);

    // set partial
    new_heaps->partial = NULL;
    // set size class pointer
    new_heaps->sc = NULL;

    return new_heaps;
}

void free_heap_desc(procheap_t *desc)
{
    if (desc == NULL)
        return;
    else
        free(desc);
}

sizeclass_t *alloc_size_class(size_t blocksize, size_t supersize)
{
    sizeclass_t *new_size;
    
    new_size = (sizeclass_t *)malloc(sizeof(sizeclass_t));

    if (new_size == NULL)
        return NULL;
    
    new_size->partial = NULL;
    new_size->sz = blocksize;
    new_size->sbsize = supersize;

    return new_size;
}

void free_size_class(sizeclass_t *desc)
{
    if (desc == NULL)
        return;
    else
        free(desc);
}

