/********************************************************************
 * DESCRIPTION : functions and data for initializing Interrupt Descriptor Table
 * FILE NAME : idt.h
 *******************************************************************/

#ifndef __IDT_H__
#define __IDT_H__

#include "irq.h"

/* 
 * Base address of IDT (Interrupt Descriptor Table) 
 */
//#define IDT_ADDRESS 0x00000000
#define IDT_ADDRESS 0xC0000000

/*
 * exception type
 */
#define DESTYPE_INT	0x8e00	/* interrupt descriptor type : 01110 */
#define DESTYPE_TRAP	0x8f00	/* trap descriptor type : 01111 */
#define DESTYPE_SYS 0xef00 // system call type

#define IDT_SIZE 256			/* length of table */

/* Interrupt Descriptor Table */
typedef struct _IDT {
	unsigned short OffsetLow;
	unsigned short SegSelector;
	unsigned short Type;
	unsigned short OffsetHigh;
} idt_t;


idt_t *idt;



/* null handler, defined in exception.asm */
extern void _ignore_isr(void);
/* Excute lidt command, in exception.asm */
extern void _lidt(void);
extern void _test_except(void);




/* initialize IDT */
void set_intr_gate(int, void *);
void set_system_gate(int, void *);
void set_trap_gate(int, void *);
void init_idt(void);
void set_idtr(void);
void init_exception(void);


extern void _divide_error(void);
extern void _debug(void);
extern void _nmi(void);
extern void _int3(void);
extern void _overflow(void);
extern void _bounds(void);
extern void _invalid_op(void);
extern void _device_not_available(void);
extern void _doublefault_fn(void);
extern void _coprocessor_segment_overrun(void);
extern void _invalid_TSS(void);
extern void _segment_not_present(void);
extern void _stack_segment(void);
extern void _general_protection(void);
extern void _page_fault(void);
extern void _coprocessor_error(void);
extern void _alignment_check(void);
extern void _machine_check(void);
extern void _simd_coprocessor_error(void);
extern void _system_call(void);


#endif
