
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "heap.h"
#include "desc.h"
#include "util.h"

#include <atomic_list.h>


/*
 * TO DO
 * 2010.1.15 17:52 making macros to access anchor here~~
 */


#define FIRST_BLOCK_SIZE 16
#define FIRST_SUPERBLOCK_SIZE (4096)

#define DPRINTF(fmt, args...) printf("[%s:%d]: " fmt, \
                                     __FUNCTION__, __LINE__, ##args)



typedef struct mem_man
{
    sizeclass_t *sizes;
    procheap_t *heaps;

    pthread_mutex_t mem_man_mutex;
    
} mem_man_t;





int init_mem_man(mem_man_t **init)
{
    int ret = 0;
    
    *init = (mem_man_t *)malloc(sizeof(mem_man_t));

    if (*init == NULL)
    {
        ret = -1;
        goto FUNC_END;
    }

    // create heap/size desc
    (*init)->heaps = NULL;
    (*init)->sizes = NULL;
    
    if (((*init)->heaps = alloc_heap_desc()) == NULL)
    {
        ret = -2;
        goto ALLOC_FAIL;
    }

    if (((*init)->sizes = alloc_size_class(FIRST_BLOCK_SIZE,
                                           FIRST_SUPERBLOCK_SIZE)) == NULL)
    {
        ret = -3;
        goto ALLOC_FAIL;
    }

    // link heap & size
    (*init)->heaps->sc = (*init)->sizes;

    if (pthread_mutex_init(&((*init)->mem_man_mutex), NULL) != 0)
    {
        ret = errno;
        goto MUTEX_FAIL;
    }
    
    return 0;
    
ALLOC_FAIL:
    if ((*init)->heaps != NULL)
        free_heap_desc((*init)->heaps);

    if ((*init)->sizes != NULL)
        free_size_class((*init)->sizes);

    free(*init);
    
    goto FUNC_END;

MUTEX_FAIL:
    if ((*init)->heaps != NULL)
        free_heap_desc((*init)->heaps);

    if ((*init)->sizes != NULL)
        free_size_class((*init)->sizes);

    free(*init);
    
FUNC_END:
    return ret;
}

void final_mem_man(mem_man_t *init)
{
    free_heap_desc(init->heaps);
    free_size_class(init->sizes);

    pthread_mutex_destroy(&(init->mem_man_mutex));

    free(init);

    return;
}


void setup_sb(void *start, int blocksize, int totalsize)
{
    int i;

    /* write small-block index */
    for (i = 0; i < totalsize / blocksize; i++)
    {
        size_t *p = (size_t *)((u64)start + i*blocksize);
        /* index of next free block must be begin at ONE,
         * becuase it means NEXT block. */
        *p = i + 1;
    }
    
}


void *alloc_newsb(procheap_t *fromheap)
{
    desc_t *new_desc;
    struct anchor_struct anchor_st;
    struct active_struct active_st;
    active_t new_active;
    anchor_t new_anchor;
    void *ret;
    
    DPRINTF("alloc from new-sb\n");


    /* desc address must be aligned to store credits in active */
    //new_desc = (desc_t *)memalign(ACTIVE_ADDR_ALIGN,
    //sizeof(desc_t));
    new_desc = get_desc();

    /* allocate new super-block */
    new_desc->sb = get_newsb(fromheap->sc->sbsize);
    setup_sb(new_desc->sb, fromheap->sc->sz, fromheap->sc->sbsize);

    /* setup descriptor */
    new_desc->size = fromheap->sc->sz;
    new_desc->maxcount = fromheap->sc->sbsize / new_desc->size;
    new_desc->heap = fromheap;

    /* setup active for heap */
    active_st.addr = new_desc;
    active_st.credits = MIN(new_desc->maxcount, MAXCREDITS) - 1;
    make_active(&active_st, &new_active);
        
    /* setup anchor for desc */
    anchor_st.avail = 1;
    anchor_st.counter = new_desc->maxcount - active_st.credits - 1;
    anchor_st.state = ANCHOR_STATE_ACTIVE;

    /* set anchor into desc */
    make_anchor(&(anchor_st), &new_anchor);
    memcpy((void *)&(new_desc->anchor),
           (void *)&new_anchor, sizeof(anchor_t));

    
    /*
     * LOCATE MEMORY FENSE HERE
     * -> why?
     */

    
    /* set active into heap -> must be done atomically */
    if (atomic_cas((void *)&(fromheap->active),
                   ZERO_ACTIVE, /* active is empty */
                   (u64)new_active) == ATOMIC_SUCCESS)
    {
        desc_t **p = (desc_t **)(new_desc->sb);
        DPRINTF("new superblock->%p\n", new_desc->sb);
        DPRINTF("new desc->%p\n", new_desc);
        DPRINTF("set new active:0x%llX\n", (u64)(new_active));
        DPRINTF("set new anchor:0x%llX\n", (u64)(new_anchor));
        
        *p = new_desc;
        ret = (void *)((desc_t **)new_desc->sb + 1);
    }
    else
    {
        /* another thread already have set new superblock,
           this thread do roll-back */
        free(new_desc->sb);
        put_desc(new_desc);
        ret = NULL;
    }
    
    return ret;
}



void *alloc_active(procheap_t *fromheap)
{
    active_t old_active, new_active;
    struct active_struct new_active_st;
    desc_t *desc;
    struct anchor_struct old_anchor_st, new_anchor_st;
    anchor_t old_anchor, new_anchor;
    
    void *addr = NULL;
    size_t next_index;
    

    //
    // <1> reserve block
    //

    // other thread can change active, this loop must read active
    // repeatly.

    DPRINTF("try alloc from active\n");
    do
    {
        // copy active field of heap
        memcpy((void *)&old_active,
               (void *)&(fromheap->active),
               sizeof(active_t));

        if (old_active == ZERO_ACTIVE)
            goto ACTIVE_FAIL;

        // copy old active into new active
        extract_active(&new_active_st, &old_active);

        // set credits
        if (ACTIVE_GET_CREDITS(old_active) == 0)
            new_active = ZERO_ACTIVE;
        else
            new_active_st.credits = new_active_st.credits - 1;

        make_active(&new_active_st, &new_active);

        DPRINTF("old active = 0x%llX new active=0x%llX\n",
                (u64)old_active, (u64)new_active);
    } while (atomic_cas((void *)&(fromheap->active),
                        old_active,
                        new_active) == ATOMIC_FAIL);

    //
    // <2> pop block
    //

    DPRINTF("new_active.addr=%p\n", ACTIVE_GET_ADDR(new_active));
    DPRINTF("new_active.credits=%llu\n", ACTIVE_GET_CREDITS(new_active));

    desc = ACTIVE_GET_ADDR(new_active);


    do
    {
        // copy anchor into old_anchor and new_anchor
        memcpy((void *)&old_anchor,
               (void *)&(desc->anchor),
               sizeof(anchor_t));
        extract_anchor(&old_anchor_st, &old_anchor);
        extract_anchor(&new_anchor_st, &old_anchor);

        DPRINTF("desc->next=%p sb=%p heap=%p size=%lu max=%lu\n",
                desc->next, desc->sb, desc->heap,
                desc->size, desc->maxcount);

        DPRINTF("anchor->tag=%lu state=%lu counter=%lu avail=%lu\n",
                old_anchor_st.tag, old_anchor_st.state,
                old_anchor_st.counter, old_anchor_st.avail);

        /* get free block */
        addr = desc->sb + old_anchor_st.avail * desc->size;

        /* set avail that are index of next free block */
        next_index = *(u64 *)addr;
        new_anchor_st.avail = next_index;
        
        DPRINTF("free block->%p next block=%lu\n", addr, next_index);

        /* increase tag for ABA problem */
        new_anchor_st.tag = old_anchor_st.tag + 1;

        /* set state & counter */
        if (ACTIVE_GET_CREDITS(old_active) == 0)
        {
            if (old_anchor_st.counter == 0)
            {
                /* no free block */
                new_anchor_st.state = ANCHOR_STATE_FULL;
            }
            else
            {
                new_anchor_st.state = old_anchor_st.state;
                new_anchor_st.counter -= MIN(old_anchor_st.counter,
                                            MAXCREDITS);
            }
        }
        else
        {
            /* do nothing */
        }

        /* building new anchor is finished */
        make_anchor(&new_anchor_st, &new_anchor);

        DPRINTF("old anchor=%llX new anchor=%llX\n",
                (u64)old_anchor, (u64)new_anchor);
    } while (atomic_cas((void *)&(desc->anchor),
                        old_anchor,
                        new_anchor) == ATOMIC_FAIL);

    /* If there is no credits, but more free block */
    

    return addr;

ACTIVE_FAIL:
    DPRINTF("alloc from active fail: no active\n");
    goto FUNC_END;

FUNC_END:
    return NULL;
}

    
void *lockfree_alloc(mem_man_t *mem_man, size_t size)
{
    procheap_t *alloc_heap;
    void *addr;
    
    //alloc_heap = find_heap(mem_man, size);
    alloc_heap = mem_man->heaps;

    if (alloc_heap == NULL)
    {
        return malloc(size);
    }

    DPRINTF("ALLOC <%d> from HEAP <%p>\n",
            (int)(alloc_heap->sc->sz),
            alloc_heap);
    
    if ((addr = alloc_active(alloc_heap)) != NULL)
        return addr;
    
    if ((addr = alloc_newsb(alloc_heap)) != NULL)
        return addr;

    return NULL;
}


int pretest(void)
{
    unsigned int i;
    int ret = 0;


    /* anchor_t processing test */
    {
        struct anchor_struct anch_st = {
            0x1234,
            0x3,
            0x11,
            0xA
        };
        anchor_t anch;
        make_anchor(&anch_st, &anch);
        extract_anchor(&anch_st, &anch);
    }

    
    /* active_t processing test */
    for (i = 0; i < 100; i++)
    {
        struct active_struct act_st;
        active_t act;
        void *p;
        
        p = memalign(4096,32);
        
        act_st.addr = p;
        act_st.credits = i;

        make_active(&act_st, &act);

        extract_active(&act_st, &act);

        if (__ull(act_st.addr) != (u64)p)
        {
            DPRINTF("ERROR TO SET ACTIVE-ADDRESS<%p != %p>\n", p,
                act_st.addr);
            free(p);
            ret = 1;
            goto PRETEST_FAIL;
        }

        if (__ull(act_st.credits) != (i & ACTIVE_CREDITS_MASK))
        {
            DPRINTF("ERROR TO SET ACTIVE-CREDITS<%lu>\n", act_st.credits);
            free(p);
            ret = 2;
            goto PRETEST_FAIL;
        }

        free(p);
    }


    return 0;
PRETEST_FAIL:
    return ret;
}


int main(void)
{
    mem_man_t *global_manager;
    void *p;

    if (pretest() != 0)
    {
        DPRINTF("pretest error\n");
        return 1;
    }


    init_mem_man(&global_manager);
    p = lockfree_alloc(global_manager, FIRST_BLOCK_SIZE);
    DPRINTF("alloc ret->%p\n", p);
    DPRINTF("desc in new block->0X%llX\n", (u64)*((desc_t **)p - 1));

    p = lockfree_alloc(global_manager, FIRST_BLOCK_SIZE);
    if (NULL != p)
    {
        DPRINTF("alloc ret->%p\n", p);
        DPRINTF("desc in new block->0X%llX\n", (u64)*((desc_t **)p - 1));
    }
    else
    {
        DPRINTF("ALLOC FAIL\n");
    }


    return 0;
}
