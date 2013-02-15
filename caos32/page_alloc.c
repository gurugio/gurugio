
#include "page_alloc.h"
#include "types.h"

#include "console.h"


//#define DEBUG 1


extern pg_data_t contig_page_data;



page_t *__alloc_pages(pg_data_t *node, unsigned int gfp_mask, unsigned int order)
{
	zone_t *zone;
	mem_map_t *map;

	unsigned long first, last;
	unsigned long count = (1UL<<order);

	
	zone = &node->node_zones;
	map = node->node_mem_map;

	for (first=0; first < zone->pages; first++) {
		// free page is found
		if (map[first].count == 0) {

			// from first to last pages are free?
			for (last=0; last<count; last++) {
				if (map[first+last].count > 0) break;
			}

			//
			// !! Page DIR/TABLE is correct? !!
			//

			// found enough space
			if (last == count) {
				// increase usage-counter of allocated pages
				for (last=0; last<count; last++) {
					map[first+last].count++;
#ifdef DEBUG
					caos_printf("page(%x) count=%d\n", &map[first+last], map[first+last].count);
#endif
				}

				// decrease free memory size
				node->node_size = node->node_size-(PAGE_SIZE*count);
				zone->size = zone->size-(PAGE_SIZE*count);
				zone->pages = zone->pages - count;

				// following pages are free, return page pointer
				return map + first;
			}

		}

	}

	return NULL;
}


extern mem_map_t *mem_map;


void __free_pages(unsigned long addr, unsigned int order)
{
	
	unsigned int pfn = __pa(addr) >> PAGE_SHIFT;
	page_t *mp = mem_map+pfn;
	int l;

#ifdef DEBUG
	caos_printf("addr=%x -> pfn=%x ", addr, pfn);
	caos_printf("mem_map+%x=%x\n", pfn, mem_map+pfn);
#endif


	for (l=0; l<(int)(1<<order); l++) {
#ifdef DEBUG
		caos_printf("mem_map(%x).count = %d -> ", mp->index, mp->count);
#endif
		mp->count--;
#ifdef DEBUG
		caos_printf("%d\n", mp->count);
#endif

	}




}



