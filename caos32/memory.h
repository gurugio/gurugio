
#ifndef	__MEMORY_H__
#define	__MEMORY_H__

#include "list.h"
#include "pgtable.h"
#include "page.h"


//
// I have only one node.
//
#define MAX_NR_ZONES 1
#define GFP_ZONEMASK 0

#define ONE_MB (1UL<<20)


typedef struct zone_struct {
	// number of free pages in zone
	unsigned long free_pages;
	// node includes zone
	struct pglist_data *zone_pgdat;
	// physical memory address
	unsigned long zone_start_paddr;
	// zone name
	char *name;
	// memory size
	unsigned long size;
	// number of total pages
	unsigned long pages;
} zone_t;



typedef struct pglist_data {
	// zone
	zone_t node_zones;
	// number of zones
	int nr_zones;
	// pointer to mem_map of this node
	struct page *node_mem_map;
	// physical memory address
	unsigned long node_start_paddr;
	// memory size
	unsigned long node_size;
	// node index
	int node_id;
	// pointer to next node
	struct pglist_data *node_next;
} pg_data_t;






void setup_memory(void);
void init_mem_map(void);
void clear_memory(char *, unsigned int);

void init_node(void);
page_t *alloc_pages(unsigned int gfp_mask, unsigned int order);
void free_pages(unsigned long addr, unsigned int order);

unsigned long get_free_pages(unsigned int gfp_mask, unsigned int order);

void print_busy_pages(void);




extern unsigned int phy_mem_size;
extern unsigned int kernel_size;


#endif
