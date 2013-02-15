#ifndef __PGTABLE_H__
#define __PGTABLE_H__

/* the page directory entry points directly to a 4MB-aligned block of 
 * memory. 
 */

#define _PAGE_BIT_PRESENT   0
#define _PAGE_BIT_RW        1
#define _PAGE_BIT_USER      2
#define _PAGE_BIT_ACCESSED  5
#define _PAGE_BIT_DIRTY     6 // only for page-table entry

#define _PAGE_PRESENT	0x001
#define _PAGE_RW    0x002
#define _PAGE_USER  0x004
#define _PAGE_ACCESSED  0x020
#define _PAGE_DIRTY 0x040


/* bit field of offset */
#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL<<PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE-1))


#define PAGE_OFFSET (0xC0000000UL)

/* bit field of page table */
#define PTE_MASK	PAGE_MASK

/* bit field of page directory */
#define PGDIR_SHIFT 22
#define PGDIR_MASK 0xFFC00000
#define PGDIR_SIZE (1UL<<PGDIR_SHIFT)

/* number of entry in page table/dir */
#define PTRS_PER_PTE 1024
#define PTRS_PER_PGD 1024

/* entry format of page-table */
typedef struct { unsigned long pte_low; } pte_t;
/* entry format of page directory */
typedef struct { unsigned long pgd; } pgd_t;

/* pgprot_t : protection bit of an entry */
typedef struct { unsigned long pgprot; } pgprot_t;


//
// access page DIR/Table entry
//
#define pte_val(x) ((x).pte_low)
#define pgd_val(x) ((x).pgd)
#define pgprot_val(x) ((x).pgprot)


#define __pte(x) ((pte_t) { (x) } )
#define __pgd(x) ((pgd_t) { (x) } )
#define __pgprot(x) ((pgprot_t) { (x) } )



//
// set page DIR/Table entry
//
#define set_pgd(pgd, entry) do { \
	((*(pgd)) = (entry)); \
} while(0)

#define set_pte(pte, entry) do { \
	((*(pte)) = (entry)); \
} while(0)




//
// Physical address <-> Virtual address
//
#define __pa(x)         ((unsigned long)(x)-PAGE_OFFSET)
#define __va(x)         ((void *)((unsigned long)(x)+PAGE_OFFSET))






#endif
