#ifndef __ASM_H__

#define __ASM_H__

#define cli() __asm__ __volatile__("cli":::"memory")
#define sti() __asm__ __volatile__("sti":::"memory")

#endif
