/******************************************************************************
 * MyOs	- my special operating system...
 *
 *
 * This is free. If you want to use and fix this program, you can do that...
 *
 * 
 * it's mine only for me...
 *
 * Inhyuk, Kim... kkojiband@hotmail.com
 *
 ******************************************************************************/


#include "gdt.h"


/* gdt 는 Setup.asm 에서 설정하며 kernel 에서는 그것을 사용한다...not goot! */
static gdt_desc_t	*gdt = (gdt_desc_t *)0x00001000;


void set_gdt_desc( int num, unsigned int addr, int size, int type )
{
	gdt[num].l	= (unsigned long)(size & 0xffff)
				| (unsigned long)(addr & 0xffff);
	
	gdt[num].h	= (unsigned long)(addr & 0xff000000)
				| (unsigned long)((addr>>16) & 0xff)
				| (unsigned long)(size & 0xf0000)
				| (unsigned long)((type<<12) & 0xf00000)
				| (unsigned long)((type<<8) & 0xff00);
}
