/********************************************************************
 * DESCRIPTION : I do not know what these function do exactly.
 *               I just copy it from several source.
 *               I hate hardware programming, 
 *               but I have studied how hardware work for 2 years
 *               to make my own OS. It is so difficult to me. ;-)
 *               The most important thing is I cannot write any
 *               description this file. :-(
 * FILE NAME : InitIRQ.c
 *******************************************************************/
#include "irq.h"


//#define DEBUG 1

unsigned short	irq_mask = 0xfffb;

#define	irq_mask0_7	(unsigned char)(irq_mask&0xff)
#define	irq_mask8_15	(unsigned char)((irq_mask>>8)&0xff)


void init_irq(void)
{
	int	i;

	init_pic();
	
	for( i = 0; i < MAX_IRQ; i++ )
		irqhandler[i]	= _ignore_irq;

	set_intr_gate(IRQ_VECTOR+0, _irq_0);
	set_intr_gate(IRQ_VECTOR+1, _irq_1);
	// IRQ2~15 is implemented later
	/*set_intr_gate(IRQ_VECTOR+2, _irq_2);
	set_intr_gate(IRQ_VECTOR+3, _irq_3);
	set_intr_gate(IRQ_VECTOR+4, _irq_4);
	set_intr_gate(IRQ_VECTOR+5, _irq_5);
	set_intr_gate(IRQ_VECTOR+6, _irq_6);
	set_intr_gate(IRQ_VECTOR+7, _irq_7);
	set_intr_gate(IRQ_VECTOR+8, _irq_8);
	set_intr_gate(IRQ_VECTOR+9, _irq_9);
	set_intr_gate(IRQ_VECTOR+10, _irq_10);
	set_intr_gate(IRQ_VECTOR+11, _irq_11);
	set_intr_gate(IRQ_VECTOR+12, _irq_12);
	set_intr_gate(IRQ_VECTOR+13, _irq_13);
	set_intr_gate(IRQ_VECTOR+14, _irq_14);
	set_intr_gate(IRQ_VECTOR+15, _irq_15);*/

}

/*
 * Initialize PIC(8259)
 * Initialize IRQ handlers to UnidentifiedIRQ function.
 * This function is called by main() function.
 */
void init_pic(void)
{
	
	/* initialize hardware interrupt base vector... */
	outb( 0x20, 0x11 );
	outb( 0xa0, 0x11 );
	
	outb( 0x21, IRQ_VECTOR );
	outb( 0xa1, IRQ_VECTOR+8 );
	
	outb( 0x21, 0x04 );
	outb( 0xa1, 0x02 );
	
	outb( 0x21, 0x01 );
	outb( 0xa1, 0x01 );
	
	outb( 0x21, irq_mask0_7 ); // mask 0~1,3~7 IRQs
	outb( 0xa1, irq_mask8_15 ); // mask 8~15 IRQs

}

/*
 * enable specified IRQ by store mask bit.
 */
void enable_irq(int irq)
{
	irq_mask	= irq_mask & ~(1<<irq);

	if( irq >= 8 )
		outb( 0xa1, irq_mask8_15 );
	else
		outb( 0x21, irq_mask0_7 );
}

/*
 * disable specified irq by set mask bit
 */
void disable_irq(int irq)
{
	irq_mask	= irq_mask | (1<<irq);

	if( irq >= 8 )
		outb( 0xa1, irq_mask8_15 );
	else
		outb( 0x21, irq_mask0_7 );
}

/*
 * register handler of specified IRQ.
 * store function pointer of handler with IRQHandler array 
 */
void register_irq(int irq, irqhandler_t handler)	
{
	irqhandler[irq]	= handler;
}

/*
 * unregister handler of specified IRQ
 * store NULL pointer with array
 */
void unregister_irq(int irq)
{
	//IRQHandler[irq]	= NULL;
	irqhandler[irq] = NULL;
}

/*
 * At fist, it send EOI signal to PIC,
 * and set mask of specified irq to disable it
 * At last because CPU enable interrups automatically
 * it enable interrupts with sti() function.
 */
void do_irq(int irq)
{

	disable_irq(irq);	// prevent irq to occur again
	outb(0x20, 0x20);	// EOI

	// enable next interrupt
	sti();

}

/*
 * If the handler of specified irq is endded,
 * it restore masking bit to enable irq.
 */
void end_irq(int irq)
{
	enable_irq(irq);
}

/*
 * When the irq is requested HandleIRQ() starts.
 * It invoke DoIRQ to enable other irqs,
 * run IRQ handler.
 * IF the handler is over, 
 * it enable the irq which are disabled by DoIRQ() at EndIRQ().
 */
void handle_irq(int irq)
{
#ifdef DEBUG
	caos_printf("IRQ[%d] occured..\n", irq);
#endif

	do_irq(irq);
	irqhandler[irq]();
	end_irq(irq);
}
