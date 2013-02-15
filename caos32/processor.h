#ifndef __PROCESSOR_H
#define __PROCESSOR_H

#include "segment.h"

typedef struct _desc_struct {
	unsigned long a,b;
} desc_struct;



/* thread structure... */
typedef struct _thread_struct
{
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int esp0;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;
	unsigned short ds;
	unsigned short ds_res;
	unsigned short es;
	unsigned short es_res;
	unsigned short fs;
	unsigned short fs_res;
	unsigned short gs;
	unsigned short gs_res;
	unsigned int	eip;
	unsigned short cs;
	unsigned short cs_res;
	unsigned int eflags;
	unsigned int esp;
	unsigned short ss;
	unsigned short ss_res;
} thread_struct;

typedef struct _tss_struct {
	unsigned short	back_link,__blh;
	unsigned long	esp0;
	unsigned short	ss0,__ss0h;
	unsigned long	esp1;
	unsigned short	ss1,__ss1h;	/* ss1 is used to cache MSR_IA32_SYSENTER_CS */
	unsigned long	esp2;
	unsigned short	ss2,__ss2h;
	unsigned long	cr3;
	unsigned long	eip;
	unsigned long	eflags;
	unsigned long	eax,ecx,edx,ebx;
	unsigned long	esp;
	unsigned long	ebp;
	unsigned long	esi;
	unsigned long	edi;
	unsigned short	es, __esh;
	unsigned short	cs, __csh;
	unsigned short	ss, __ssh;
	unsigned short	ds, __dsh;
	unsigned short	fs, __fsh;
	unsigned short	gs, __gsh;
	unsigned short	ldt, __ldth;
	unsigned short	trace, io_bitmap_base;
} __attribute__((packed)) tss_struct;



#endif /* __PROCESSOR_H */
