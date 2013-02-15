
#include "types.h"
#include "page.h"

#include "console.h"


pg_data_t contig_page_data;


unsigned long get_phymem_size(void)
{
	return 32;
}

void init_paging(void)
{
	caos_printf("### TEST PAGING ###\n");

	contig_page_data.node_zones.free_pages = 8*1024;
	contig_page_data.node_zones.size = 32;
	contig_page_data.node_zones.zone_pgdat = &contig_page_data;
	contig_page_data.node_zones.zone_start_paddr = 0x0;
	contig_page_data.node_zones.name = "NORMAL";

	contig_page_data.nr_zones = MAX_NR_ZONES;
	contig_page_data.node_mem_map = NULL;
	contig_page_data.node_start_paddr = 0;
	contig_page_data.node_size = 32;
	contig_page_data.node_id = 0;
	contig_page_data.node_next = NULL;

}
