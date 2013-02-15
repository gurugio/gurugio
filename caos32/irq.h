/********************************************************************
 * DESCRIPTION : I do not know what these function do exactly.
 *               I just copy it from several source.
 *               I hate hardware programming, 
 *               but I have studied how hardware work for 2 years
 *               to make my own OS. It is so difficult to me. ;-)
 *               The most important thing is I cannot write any
 *               description this file. :-(
 * FILE NAME : IRQ.h
 *******************************************************************/
#ifndef __IRQ_H__

#define __IRQ_H__

#include "types.h"
#include "io.h"
#include "idt.h"
#include "inline_asm.h"
#include "console.h"

#define MAX_IRQ 16
#define IRQ_VECTOR 0x20

#define IRQ_TIMER 0x0
#define IRQ_KEYBOARD 0x1


//typedef void (*IRQHandler_t)(void);
typedef void (*irqhandler_t)(void);

void init_pic(void);
void init_interrupt(void);
void init_irq(void);

void enable_irq(int);
void disable_irq(int);
void do_irq(int);
void register_irq(int, irqhandler_t);
void unregister_irq(int);
void handle_irq(int);


//extern void EnableIRQ(int);
//extern void DisableIRQ(int);
//extern void RegisterIRQHandler(int, IRQHandler_t);
//extern void UnregisterIRQHandler(int);
//extern void UnidentifiedIRQ(void);

//extern void HandleIRQ(int);

extern void _ignore_irq(void);
extern void _irq_0(void);
extern void _irq_1(void);


#define DECLARE_IRQ(num)	\
	extern void handle_irq_##num(void);	\
	set_intr_gate(IRQ_VECTOR+num, handle_irq_##num)

//static IRQHandler_t	IRQHandler[MAX_IRQ];
irqhandler_t irqhandler[MAX_IRQ];


#endif
