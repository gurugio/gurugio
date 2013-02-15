/********************************************************************
 * DESCRIPTION : port I/O
 * FILE NAME : IO.h
 *******************************************************************/

#ifndef __IO_H__

#define __IO_H__

extern unsigned char inb(unsigned short);
extern unsigned short inw(unsigned short);

extern void outb(unsigned short, unsigned char);
extern void outw(unsigned short, unsigned short);

#endif
