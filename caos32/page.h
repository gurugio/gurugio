
#ifndef	__PAGE_H__
#define	__PAGE_H__


#include "list.h"
#include "pgtable.h"



/* mem_mep_t presents a page frame */
typedef struct page {
	struct list_head list;
	unsigned long index;
	int count;
	unsigned long flags;
} mem_map_t;

typedef struct page page_t;


void paging_init(unsigned int);

void bootmem_init(unsigned int);
void set_boot_bitmap(int);
void *alloc_bootmem(void);

page_t *virt_to_page(unsigned long vaddr);
unsigned long page_to_virt(page_t *);

#endif
