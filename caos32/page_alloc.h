#ifndef __PAGE_ALLOC_H__
#define __PAGE_ALLOC_H__

#include "memory.h"


#define MAX_PAGE_ORDER 0x3


#define __GFP_KERNEL	((unsigned int)0x8000)
#define __GFP_USER		((unsigned int)0x4000)





page_t *__alloc_pages(pg_data_t *, unsigned int, unsigned int);

void *__get_free_pages(unsigned int, unsigned int);


void __free_pages(unsigned long vaddr, unsigned int order);





#endif
