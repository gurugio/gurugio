
/********************************************************************
 * DESCRIPTION :
 * FILE NAME : timer.h
 *******************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "io.h"
#include "irq.h"

void init_timer(void);
void timer_isr(void);

int timer_open(const char *);
int timer_ioctl(unsigned int cmd);


#endif
