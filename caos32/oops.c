#include "oops.h"


void oops(unsigned int data)
{
	static char hex[16] = "0123456789abcdef";
	char *p = (char *)0xb8000;

	unsigned int regs[9];
	
	char *name[] = { "eax = ", "ebx = ", "ecx = ", "edx = ", "eip = ",
					 "esp = ", "cr3 = ", "flag= ", "etc =" };

	int	i, j, c;


	cli();

	asm __volatile__ (
			"movl %%eax, %0\n"
			"movl %%ebx, %1\n"
			"movl %%ecx, %2\n"
			"movl %%edx, %3\n"
			"movl %%esp, %4\n"
			"movl %%cr3, %%eax\n"
			"movl %%eax, %5\n"
			"pushfl\n"
			"movl (%%esp), %%eax\n"
			"movl %%eax, %6\n"
			: "=m" (regs[0]), "=m" (regs[1]), "=m" (regs[2]), "=m" (regs[3]),
			  "=m" (regs[5]), "=m" (regs[6]), "=m" (regs[7])
			);

	regs[4]	= *(unsigned int *)((unsigned int *)&data - 1);
	
	regs[8]	= data;

	for( i = 0; i < 160*9; i += 160 )
	{
		for( j = i; j < i+12; j += 2 )
			p[j]	= name[i/160][(j-i)/2];
		
		c	= 7;
		for( j = i+12; j < i+28; j += 2 )
		{
			p[j]	= hex[ ( regs[i/160] >> (c*4) ) & 0xf ];
			c--;
		}

		p[j]	= ' ';
	}

	while(1);
}
