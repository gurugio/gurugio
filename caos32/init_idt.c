/********************************************************************
 * DESCRIPTION : initialize Interrupt Descriptor Table
 * FILE NAME : Initidt.c
 *******************************************************************/

/* header file for initializing idts */
#include "idt.h"
#include "irq.h"
#include "segment.h"

#include "console.h"

/******************************************************************
 * NAME : SetIntrGate
 * SYNOPSIS : initialize interrupt gate
 * EXTERNAL EFFECTS : idt[]
 * ARGUMENTS : A_iIntNum interrupt number
 *             A_pHandler pointer of handler function
 * RETURNS : N/A
 * ERRNOS : N/A
 ******************************************************************/
void set_intr_gate(int A_nIntNum, void *A_pHandler)
{
		if (A_nIntNum < IDT_SIZE) {
				idt[A_nIntNum].OffsetHigh = (unsigned short)(((unsigned long)A_pHandler & 0xffff0000) >> 16);
				idt[A_nIntNum].Type = DESTYPE_INT;		/* interrupt descriptor */
				idt[A_nIntNum].SegSelector = __KERNEL_CS;	/* kernel code segment */
				idt[A_nIntNum].OffsetLow = (unsigned short)((unsigned long)A_pHandler & 0xffff);
		} 
}

/******************************************************************
 * NAME : SetSystemrGate
 * SYNOPSIS : initialize system gate
 * EXTERNAL EFFECTS : idt[]
 * ARGUMENTS : A_nIntNum exception number
 *             A_pHandler address of exception handler
 * RETURNS : N/A
 * ERRNOS : N/A
 ******************************************************************/
void set_system_gate(int A_nIntNum, void *A_pHandler)
{
		if (A_nIntNum < IDT_SIZE) {
				idt[A_nIntNum].OffsetHigh = (unsigned short)(((unsigned long)A_pHandler & 0xffff0000) >> 16);
				//
				// System gate is allowed for user task to occur exception
				// System call and some exceptions should be defined as system gate
				//
				idt[A_nIntNum].Type = DESTYPE_SYS;			/* descriptor type */
				idt[A_nIntNum].SegSelector = __KERNEL_CS;	/* kernel code segment */
				idt[A_nIntNum].OffsetLow = (unsigned short)((unsigned long)A_pHandler & 0xffff);
		} 
}

/******************************************************************
 * NAME : SetTrapGate
 * SYNOPSIS : initialize trap gate
 * EXTERNAL EFFECTS : idt[]
 * ARGUMENTS : A_nIntNum trap number
 *             A_pHandler address of trap handler
 * RETURNS : N/A
 * ERRNOS : N/A
 ******************************************************************/
void set_trap_gate(int A_nIntNum, void *A_pHandler)
{
		if (A_nIntNum < IDT_SIZE) {
				idt[A_nIntNum].OffsetHigh = (unsigned short)(((unsigned long)A_pHandler & 0xffff0000) >> 16);
				idt[A_nIntNum].Type = DESTYPE_TRAP;			/* trap descriptor */
				idt[A_nIntNum].SegSelector = __KERNEL_CS;	/* kernel code segment */
				idt[A_nIntNum].OffsetLow = (unsigned short)((unsigned long)A_pHandler & 0xffff);
		} 
}



/******************************************************************
 * NAME : Initidt
 * SYNOPSIS : initialize INTERRUPT DESCRIPTOR TABLE
 * EXTERNAL EFFECTS : N/A
 * ARGUMENTS :N/A
 * RETURNS : N/A
 * ERRNOS : N/A
 ******************************************************************/
void init_idt(void)
{
	int nIndex = 0;
	
	caos_printf("Init IDT..\n");
	idt = (idt_t *)IDT_ADDRESS;

	/* initialize all idt with null handler */
	for (nIndex = 0; nIndex < IDT_SIZE; nIndex++) {
		set_trap_gate(nIndex, _ignore_isr);
	}

	init_exception();
	init_irq();

}

void set_idtr(void)
{
	_lidt();
}


void init_exception(void)
{

	set_trap_gate(0, _divide_error);
	set_trap_gate(1, _debug);
	set_intr_gate(2, _nmi);
	set_system_gate(3, _int3);
	set_system_gate(4, _overflow);
	set_system_gate(5, _bounds);
	set_trap_gate(6, _invalid_op);
	set_trap_gate(7, _device_not_available);
	set_trap_gate(8, _doublefault_fn);
	set_trap_gate(9, _coprocessor_segment_overrun);
	set_trap_gate(10, _invalid_TSS);
	set_trap_gate(11, _segment_not_present);
	set_trap_gate(12, _stack_segment);
	set_trap_gate(13, _general_protection);
	set_intr_gate(14, _page_fault);
	set_trap_gate(16, _coprocessor_error);
	set_trap_gate(17, _alignment_check);
	set_trap_gate(18, _machine_check);
	set_trap_gate(19, _simd_coprocessor_error);
	set_system_gate(128, _system_call);


}

