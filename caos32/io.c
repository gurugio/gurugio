/********************************************************************
 * DESCRIPTION : port I/O function
 * FILE NAME : IO.c
 *******************************************************************/
#include "io.h"


/*
 * gcc inline assembly indentifier
 * Output/Input list
 * = : value will be changed
 * a : ax
 * d : dx
 * N : 0~255
 */

/****************************************************************
 * NAME : inb
 * SYNOPSIS : in byte
 * EXTERNAL EFFECTS : N/A
 * ARGUMENTS : port  port number 
 * RETURNS : _v read value from AL
 * ERRNOS : N/A
 ***************************************************************/
unsigned char inb(unsigned short port)
{
	unsigned char _v;
	__asm__ __volatile__ ("inb %w1, %0":"=a" (_v):"Nd" (port));
	return _v;
}

/****************************************************************
 * NAME : inw
 * SYNOPSIS : in word
 * EXTERNAL EFFECTS : N/A
 * ARGUMENTS : port  port number
 * RETURNS : _v read value from word
 * ERRNOS : N/A
 ***************************************************************/
unsigned short inw(unsigned short port)
{
	unsigned short _v;
	__asm__ __volatile__ ("inw %w1,%0":"=a" (_v):"Nd"(port));
	return _v;
}

/****************************************************************
 * NAME : outb
 * SYNOPSIS : out byte
 * EXTERNAL EFFECTS : N/A
 * ARGUMENTS : port  port number
 *             value  write value
 * RETURNS : N/A
 * ERRNOS : N/A
 ***************************************************************/
void outb(unsigned short port, unsigned char value)
{
	__asm__ __volatile__ ("outb %b0,%w1": :"a" (value), "Nd"(port));
}

/****************************************************************
 * NAME : outw
 * SYNOPSIS : out word
 * EXTERNAL EFFECTS : N/A
 * ARGUMENTS : port  port number
 *             value write value
 * RETURNS : N/A
 * ERRNOS : N/A
 ***************************************************************/
void outw(unsigned short port, unsigned short value)
{
	__asm__ __volatile__ ("outw %w0,%w1": :"a" (value), "Nd"(port));
}


