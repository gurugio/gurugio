
#include "types.h"
#include "memory.h"
#include "page_alloc.h"


#include "console.h"


//#define DEBUG 1



pg_data_t contig_page_data;
unsigned int phy_mem_size;
mem_map_t *mem_map;
unsigned int kernel_size;



void setup_memory(void)
{
#ifdef DEBUG
	caos_printf("### MEMORY MANAGEMENT ###\n");
#endif

	caos_printf("memory size = %dMB, kernel size = %dbytes\n", phy_mem_size, kernel_size);

	//
	// memory allocator for booting process
	//
	bootmem_init(kernel_size);

	//
	// First of all, page management is initialized.
	// 
	paging_init(phy_mem_size);

	//
	// Build page mapping table, mem_map
	//
	init_mem_map();
	init_node();


}





void init_node(void)
{

	contig_page_data.node_zones.free_pages = phy_mem_size*ONE_MB/PAGE_SIZE;
	contig_page_data.node_zones.size = phy_mem_size;
	contig_page_data.node_zones.zone_pgdat = &contig_page_data;
	contig_page_data.node_zones.zone_start_paddr = 0x0;
	contig_page_data.node_zones.name = "NORMAL";

	contig_page_data.node_zones.pages = (phy_mem_size * ONE_MB)/PAGE_SIZE; 
	


	contig_page_data.nr_zones = MAX_NR_ZONES;
	contig_page_data.node_mem_map = mem_map;
	contig_page_data.node_start_paddr = 0;
	contig_page_data.node_size = phy_mem_size;
	contig_page_data.node_id = 0;
	contig_page_data.node_next = NULL;

}

#ifdef DEBUG
extern unsigned char boot_bitmap[];
#endif

void init_mem_map(void)
{
	unsigned int page_count;

	int mem_map_size;
	int mem_map_page;
	int l;
	unsigned int tmp;


	page_count = (phy_mem_size * ONE_MB)/PAGE_SIZE;
	mem_map_size = page_count * sizeof(mem_map_t);
	mem_map_page = mem_map_size / PAGE_SIZE;

	caos_printf("page count = %d, mem_map_size = %d, mem_map_page = %d\n", page_count, mem_map_size, mem_map_page);


	// 
	// memory space for mem_map, mem_map_page pages
	//
	mem_map = (mem_map_t *)alloc_bootmem();
	caos_printf("mem_map -> %x ", (unsigned long)mem_map);
	for (l=0; l<mem_map_page-1; l++) {
		caos_printf("%x ", (unsigned long)alloc_bootmem());
	}
	caos_printf("\n");

#ifdef DEBUG
	caos_printf("\n");
	caos_printf("boot_bitmap = ");
	for (l=0; l<0x10; l++)
		caos_printf("%x ", boot_bitmap[l]);
	caos_printf("\n");
#endif


	for (l=0; l<page_count; l++) {
		INIT_LIST_HEAD(&mem_map[l].list);
		mem_map[l].index = l;
		mem_map[l].count = 0;
		mem_map[l].flags = 0;
	}
	
#ifdef DEBUG
	caos_printf("mem_map[%d] : ", mem_map[0].index);
	caos_printf("list.next=%x list.prev=%x ", mem_map[0].list.next, mem_map[0].list.prev);
	caos_printf("count=%d\n", mem_map[0].count);
	
	caos_printf("mem_map[%d] ", mem_map[page_count-1].index);
	caos_printf("list.next=%x list.prev=%x ", mem_map[page_count-1].list.next, mem_map[page_count-1].list.prev);
	caos_printf("count=%d\n", mem_map[page_count-1].count);
#endif


	// check boot_bitmap for booting memory allocation
	tmp = (unsigned int)alloc_bootmem();
	for (l=0; l<tmp/PAGE_SIZE; l++)
		mem_map[l].count = 1;

	// kernel : 0x100000(0x100th page)~
	for (l=0x100; l<(kernel_size/PAGE_SIZE)+1; l++)
		mem_map[l].count = 1;
	
#ifdef DEBUG
	print_busy_pages();
#endif


}

void print_busy_pages(void)
{
	int l;
	int page_count = (phy_mem_size * ONE_MB)/PAGE_SIZE;

	caos_printf("Busy Pages : ");
	for (l=0; l<page_count; l++) {
		if (mem_map[l].count > 0)
			caos_printf("%x ", l);
		else
			caos_printf("[%x] ", l);
	}
	caos_printf("\n");

}


page_t *alloc_pages(unsigned int gfp_mask, unsigned int order)
{
	if (order > MAX_PAGE_ORDER)
		return NULL;
	
	//
	// gfp_mask processing should be here
	//
	
	return __alloc_pages(&contig_page_data, gfp_mask, order);

}


unsigned long get_free_pages(unsigned int gfp_mask, unsigned int order)
{
	page_t *pg;


	if (order > MAX_PAGE_ORDER)
		return (unsigned long)NULL;

	pg = __alloc_pages(&contig_page_data, gfp_mask, order);
	
#ifdef DEBUG
	caos_printf("get_free_pages : %x..OK\n", (unsigned long)pg->index);
#endif

	return page_to_virt(pg);
}




void free_pages(unsigned long addr, unsigned int order)
{

	if (order > MAX_PAGE_ORDER)
		return;


	__free_pages(addr, order);

}








