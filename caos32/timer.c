#include "timer.h"
#include "console.h"

#include "sched.h"
#include "char_device.h"

#include "string.h"


unsigned long timer_count;
unsigned short timer_interval;

char timer_msg[] = "[TIMER ISR] ";
char flow[4] = {'-', '\\', '|', '/'};

char timer_device_name[] = "timer";
char_device_struct_t timer_dev;
file_operations_t timer_ops;


void init_timer(void)
{
	int major;

	timer_dev.name = timer_device_name;
	timer_dev.fops = &timer_ops;

	major = register_chrdev(timer_device_name, &timer_dev);
	
	if (major < 0)
		return;

	timer_dev.major = major;

	timer_ops.read = NULL;
	timer_ops.write = NULL;
	timer_ops.open = timer_open;
	timer_ops.close = NULL;
	timer_ops.ioctl = timer_ioctl;
	
	caos_printf("Initialize timer..");

	/* 
	 * Timer hardware initializing must be added here 
	 */

	timer_interval = 0xffff; // maximum interval
	outb(0x43, 0x34);
	outb(0x40, (unsigned char)(timer_interval & 0xff));
	outb(0x40, (unsigned char)((timer_interval>>8) & 0xff));


	register_irq(0, timer_isr);
	enable_irq(0);

	timer_count = 0;

	clear_screen(0, SCREEN_COL);
	caos_printf("OK\n");
}

int timer_open(const char *name)
{
	
	return 0;
}

int timer_ioctl(unsigned int cmd)
{

	return 0;
}



void timer_isr(void)
{

	/* 
	 * Timer hardware configuration must be added here 
	 */

	timer_count++;

	timer_msg[11] = flow[timer_count%4];

	write_screen(timer_msg, 22, 12);

}
