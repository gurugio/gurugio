
#include "types.h"
#include "page.h"

#include "console.h"


//#define DEBUG 1



#define SWAPPER_PG_DIR_ADDR 0xc0003000
#define PTE0 0xc0004000

#define BOOT_BITMAP_SIZE 20



//
// bitmap for low memory under 0xA0000 (1024pages)
// 1bit -> 1page
// If bit is 1, the page is used.
// When mem_map is initialized, 
// this bitmap presents which pages are allocated to the kernel and which pages are free.
//
unsigned char boot_bitmap[BOOT_BITMAP_SIZE];

//
// Page Global Dir for kernel
//
pgd_t *swapper_pg_dir;



void set_boot_bitmap(int index)
{
	int byte, bit;
	unsigned char mask = 0x80;

	byte = index/8;
	bit = index%8;

	boot_bitmap[byte] |= (mask>>bit);

}


void bootmem_init(unsigned int kernel_size)
{
	int l;
	for (l=0; l<BOOT_BITMAP_SIZE; l++)
		boot_bitmap[l] = 0;

	//
	// 0th ~ 0x7th pages are used for PGD/PTE
	// 100th ~ kernel_size/PAGE_SIZE+1 are used for the kernel
	//
	for (l=0; l<= 0x7; l++)
		set_boot_bitmap(l);

}

//
// only one page allocating
//
void *alloc_bootmem(void)
{
	int l=0;
	while (boot_bitmap[l] == 0xff)
		l++;
	
	//
	// booting memory should be allocated under video mem, 0xb8000
	//
	if (l >= 0xa0)
		return NULL;

	if (boot_bitmap[l] == 0x0) {
		set_boot_bitmap(l*8);
		return (void *)((l*8)*PAGE_SIZE);
	} else if (boot_bitmap[l] == 0x80) {
		set_boot_bitmap(l*8+1);
		return (void *)((l*8+1)*PAGE_SIZE);
	} else if (boot_bitmap[l] == 0xc0) {
		set_boot_bitmap(l*8+2);
		return (void *)((l*8+2)*PAGE_SIZE);
	} else if (boot_bitmap[l] == 0xe0) {
		set_boot_bitmap(l*8+3);
		return (void *)((l*8+3)*PAGE_SIZE);
	} else if (boot_bitmap[l] == 0xf0) {
		set_boot_bitmap(l*8+4);
		return (void *)((l*8+4)*PAGE_SIZE);
	} else if (boot_bitmap[l] == 0xf8) {
		set_boot_bitmap(l*8+5);
		return (void *)((l*8+5)*PAGE_SIZE);
	} else if (boot_bitmap[l] == 0xfc) {
		set_boot_bitmap(l*8+6);
		return (void *)((l*8+6)*PAGE_SIZE);
	} else if (boot_bitmap[l] == 0xfe) {
		set_boot_bitmap(l*8+7);
		return (void *)((l*8+7)*PAGE_SIZE);
	}

	return NULL;

}



void paging_init(unsigned int phy_mem_size)
{
	int l;
	pte_t *pte;
	pgd_t *pgd;
	unsigned long addr;
	unsigned long pte_start;
	int pgd_count;


#ifdef DEBUG
	caos_printf("Init Page Global Dir\n");
#endif

	swapper_pg_dir = (pgd_t *)SWAPPER_PG_DIR_ADDR;

	// PRINT PAGE DIR ENTRY!!
	//
#ifdef DEBUG
	caos_printf("## PGD check : %x -> 0x23=accessed, su, R/W, Pre\n", swapper_pg_dir[0] ); 
	

	caos_printf("## Physical memory 0~4MB is mapping to 0xC0000000~0xC0400000\n");
	caos_printf("0xB8000 -> 0xC00B8000, First char of scrren C->A\n");
	do { char *t=(char *)0xC00b8000; *t = 'A'; } while (0);
#endif

	//
	// swapper_pg_dir[300] is already allocated for 0~4MB in setup.asm
	// This is mapping PAGE DIR for 4MB~ end of physical memory
	//
	pgd = swapper_pg_dir+0x301;
	pte_start = (unsigned long)alloc_bootmem();
	addr = pte_start + (_PAGE_RW | _PAGE_PRESENT);
	pgd_count = phy_mem_size/4;

	for (l=1; l<pgd_count; l++) {
		set_pgd(pgd, __pgd(addr));
#ifdef DEBUG
		caos_printf("swapper[%d]=%x ", l, addr);
#endif
		addr = (unsigned long)alloc_bootmem() + (_PAGE_RW|_PAGE_PRESENT);
		pgd++;
	}

#ifdef DEBUG
	for (l=0x2ff; l<0x309; l++)
		caos_printf("swapper_pg_dir[%d]=%x  ", l, swapper_pg_dir[l]);
	caos_printf("\n");
#endif

	//
	// mapping PAGE TABLE for 4MB ~ end of physical memory
	//
	pte = (pte_t *)pte_start;	// page table at 0x5000;
	addr = (4*0x100000) + (_PAGE_RW | _PAGE_PRESENT);	// 4MB +
	pgd_count = phy_mem_size/4;

	for (l=1024; l<PTRS_PER_PTE*pgd_count; l++) {	// fill out 7 tables
		set_pte(pte, __pte(addr));
		addr += PAGE_SIZE;
		pte++;
	}

#ifdef DEBUG
	caos_printf("TEST PHY mapping..");
	do {
		char *pt;
		for (pt = (char *)0xC0400000; pt<(char *)0xC0000000+phy_mem_size*0x100000; pt+=0x100000)
			*pt = 'a';
		caos_printf("MEM test %x ", (unsigned long)pt);
		if (*pt == 'a')
			caos_printf("OK\n");
		else
			caos_printf("FAIL\n");
	} while (0);
#endif
}




extern mem_map_t *mem_map;


page_t *virt_to_page(unsigned long vaddr)
{
	unsigned int pfn = __pa(vaddr) >> PAGE_SHIFT;

#ifdef DEBUG
	caos_printf("virt (%x) to page #(%x)\n", vaddr, pfn);
#endif
	return mem_map+pfn;
}


unsigned long page_to_virt(page_t *pg)
{
	unsigned int pfn = pg - mem_map;
#ifdef DEBUG
	caos_printf("page #(%x) to virt (%x)\n", pfn, __va(pfn<<PAGE_SHIFT));
#endif
	return (unsigned long)__va(pfn << PAGE_SHIFT);
}











