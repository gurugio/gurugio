
#ifndef	__GDT_H__
#define	__GDT_H__



/* gdt descriptor is 4 bytes... */
typedef struct _gdtDesc		gdt_desc_t;
struct _gdtDesc
{
	unsigned long	l, h;
};


extern void set_gdt_desc( int, unsigned int, int, int );

/* set task state segment... */
#define	set_tss_desc(num,addr)	set_gdt_desc(num,addr,235,0x89)


#endif
