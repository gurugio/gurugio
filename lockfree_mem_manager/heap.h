
#ifndef __HEAP_H__
#define __HEAP_H__



#include <stdio.h>

#include "util.h"
#include "desc.h"


/* 64bit field to bit in one atomic block
 * [64:8] - address of active block
 * [7:0] - credits
 */
typedef volatile unsigned long long active_t;

#define ACTIVE_CREDITS_MASK 0xFFLLU
#define ACTIVE_SET_CREDITS(active,credits) \
    (active) = (active_t)((__ull(active) & ~ACTIVE_CREDITS_MASK) |   \
                (__ull(credits) & ACTIVE_CREDITS_MASK))
#define ACTIVE_GET_CREDITS(active) \
    __ull(__ull(active) & ACTIVE_CREDITS_MASK)


#define ACTIVE_ADDR_MASK (~ACTIVE_CREDITS_MASK)
#define ACTIVE_ADDR_BIT (8)
#define ACTIVE_ADDR_ALIGN (0x1 << ACTIVE_ADDR_BIT)

#define ACTIVE_SET_ADDR(active,addr)                                        \
    (active) = (active_t)((__ull(active) & ~ACTIVE_ADDR_MASK) | __ull(addr))
#define ACTIVE_GET_ADDR(active) \
    (desc_t *)(__ull(active) & ACTIVE_ADDR_MASK)


#define ZERO_ACTIVE (0)
#define MAXCREDITS (10) /*  */



typedef struct sizeclass
{
    desc_t *partial;
    size_t sz;        // block size
    size_t sbsize;    // super-block size
} sizeclass_t;

typedef struct procheap
{
    active_t active;         // address of active block
    desc_t *partial; // address of partial block
    sizeclass_t *sc; // address of size class
} procheap_t;


struct active_struct
{
    void *addr;
    size_t credits;
};



procheap_t *alloc_heap_desc(void);
void free_heap_desc(procheap_t *desc);
sizeclass_t *alloc_size_class(size_t blocksize, size_t supersize);
void free_size_class(sizeclass_t *desc);

inline void make_active(struct active_struct *act_struct, active_t *act);
inline void extract_active(struct active_struct *act_struct,
                           active_t *act);



#endif
