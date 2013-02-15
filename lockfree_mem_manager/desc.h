#ifndef __DESC_H__
#define __DESC_H__


#include <stdio.h>

#include "util.h"


/* 64bit field to fit in one atomic block
 * [63:54] - index of available block
 * [53:44] - free block counter
 * [43:42] - state
 * [41:0]  - tagging counter to avoid ABA problem
 */
typedef volatile u64 anchor_t;

#define ANCHOR_TAG_BIT 0ULL
#define ANCHOR_STATE_BIT 42ULL
#define ANCHOR_COUNTER_BIT 44ULL
#define ANCHOR_AVAIL_BIT 54ULL

#define ANCHOR_TAG_MASK 0x3FFFFFFFFFFLLU
#define ANCHOR_STATE_MASK __ull((__ull(0x3) << ANCHOR_STATE_BIT))
#define ANCHOR_COUNTER_MASK __ull((__ull(0x3FF) << ANCHOR_COUNTER_BIT))
#define ANCHOR_AVAIL_MASK __ull((__ull(0x3FF) << ANCHOR_AVAIL_BIT))

#define ANCHOR_GET_TAG(anchor) \
    __ull(__ull(anchor) & ANCHOR_TAG_MASK)
#define ANCHOR_GET_STATE(anchor) \
    ((__ull(__ull(anchor) & ANCHOR_STATE_MASK)) >> ANCHOR_STATE_BIT)
#define ANCHOR_GET_COUNTER(anchor) \
    ((__ull(__ull(anchor) & ANCHOR_COUNTER_MASK)) >> ANCHOR_COUNTER_BIT)
#define ANCHOR_GET_AVAIL(anchor) \
    ((__ull(__ull(anchor) & ANCHOR_AVAIL_MASK)) >> ANCHOR_AVAIL_BIT)



enum
{
    ANCHOR_STATE_ACTIVE = 0,
    ANCHOR_STATE_FULL,
    ANCHOR_STATE_PARTIAL,
    ANCHOR_STATE_EMPTY
};


/*
#define ANCHOR_SET_TAG(anchor,tag) \
    (anchor) = (anchor_t)((__ull(anchor) & ~ANCHOR_TAG_MASK) | \
                          (__ull((tag) & ANCHOR_TAG_MASK))
#define ANCHOR_SET_STATE(anchor,stat)               \
    (anchor) = (anchor_t)((__ull(anchor) & ~ANCHOR_STATE_MASK) | (stat << ANCHOR_STATE_BIT))
*/
    


typedef struct desc
{
    anchor_t anchor; // descriptor information
    struct desc *next; // next super-block
    void *sb;                // pointer to chunk memory
    struct procheap *heap;   // pointer to owner processor-heap
    size_t size;             // block size
    size_t maxcount;         // superblock size / block size
} desc_t;


struct anchor_struct
{
    size_t tag;
    size_t state;
    size_t counter;
    size_t avail;
};


inline void extract_anchor(struct anchor_struct *anc_struct,
                           anchor_t *anc);
inline void make_anchor(struct anchor_struct *anc_struct, anchor_t *anc);
desc_t *get_desc(void);
void put_desc(desc_t *desc);
void *get_newsb(size_t newsb_size);

#endif
