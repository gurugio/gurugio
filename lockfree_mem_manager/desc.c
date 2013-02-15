


#include <stdio.h>
#include <malloc.h>


#include "desc.h"
#include "heap.h"



inline void extract_anchor(struct anchor_struct *anc_struct,
                    anchor_t *anc)
{
    anc_struct->tag = ANCHOR_GET_TAG(*anc);
    anc_struct->state = ANCHOR_GET_STATE(*anc);
    anc_struct->counter = ANCHOR_GET_COUNTER(*anc);
    anc_struct->avail = ANCHOR_GET_AVAIL(*anc);
}


inline void make_anchor(struct anchor_struct *anc_struct, anchor_t *anc)
{
    size_t tag, state, counter, avail;
    
    tag = (anc_struct->tag << ANCHOR_TAG_BIT) & ANCHOR_TAG_MASK;
    state = (anc_struct->state << ANCHOR_STATE_BIT) & ANCHOR_STATE_MASK;
    counter = (anc_struct->counter << ANCHOR_COUNTER_BIT) & \
        ANCHOR_COUNTER_MASK;
    avail = (anc_struct->avail << ANCHOR_AVAIL_BIT) & ANCHOR_AVAIL_MASK;

    (*anc) = tag | state | counter | avail;
}


desc_t *get_desc(void)
{
    /* BUGBUG: locking of memory manager can be here */
    
    return memalign(ACTIVE_ADDR_ALIGN, sizeof(desc_t));
}

void *get_newsb(size_t newsb_size)
{
    /* BUGBUG: no need to lock? */
       
    return malloc(newsb_size);
}

void put_desc(desc_t *desc)
{
    /* BUGBUG: locking of memory manager can be here */
    
    free(desc);
}
