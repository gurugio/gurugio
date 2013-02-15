

#include "syscall.h"
#include "console.h"
#include "process.h"
#include "inline_asm.h"
#include "memory.h"
#include "sched.h"
#include "keyboard.h"
#include "string.h"



int do_syscall_test(void)
{
	caos_printf("system-call test\n");
	return 1;
}


int do_cputstr(const char *str)
{
	caos_printf(str);
	return 1;
}


int do_cgetstr(char *buf, unsigned long length)
{
	int index;

	if (length > KEY_INPUT_MAX) {
		caos_printf("sys_cgetstr error: string length\n");
		return -1;
	}

	/*index = 0;
	for (index=0; index<fore_task->key_offset; index++) {
		buf[index] = fore_task->key_input[index];

		if (index > length)
			break;
	}*/
	keyboard_read(buf, length, 0);

	fore_task->key_offset = 0;

	return index;	// return-value is string length
}



int do_cprintf(const char *format, unsigned long arg)
{
	caos_printf(format, arg);
	return 1;
}


int do_system_info(void)
{
	caos_printf("System Information: \n");
	caos_printf("Physical Memory : %d MB\n", phy_mem_size);
	caos_printf("CaOS kernel : %d byte\n", kernel_size);

	return 0;
}

int do_turnoff(void)
{
	del_task(1);
	del_task(2);
	stop_scheduler();
	cli();
	return 0;

}


int do_open(const char *name, int flag)
{

	if (caos_strcmp(name, "keyboard") == 0) {
		return keyboard_open(name);
	} else {
		caos_printf("do_open - Unknown name: %x\n", name);

	}
	return 0;
}

int do_close(int fd)
{

	if ( fd == 0) {
		return keyboard_close("keyboard");
	} else {
		caos_printf("do_close - Unknown fd: %d\n", fd);
	}

	return 0;
}

int do_read(int fd, char *buf, int count)
{

	return keyboard_read(buf, count, 0);

}

int do_write(int fd, char *buf, int count)
{
	caos_printf("do_write - fd:%d, buf:%x, count:%d\n", fd, buf, count);

	return 0;
}




//
// Every system-call returns integer value
// and no limitation for parameters
typedef int (*sys_call_handler)(void);

//
// system-call table has pointers of system-call function
//
sys_call_handler sys_call_table[] = {
	/* #0 */ (sys_call_handler)do_syscall_test,
	/* #1 */ (sys_call_handler)do_cputstr,
	/* #2 */ (sys_call_handler)do_cgetstr,
	/* #3 */ (sys_call_handler)do_cprintf,
	/* #4 */ (sys_call_handler)do_system_info,
	/* #5 */ (sys_call_handler)do_turnoff,
	/* #6 */ (sys_call_handler)do_open,
	/* #7 */ (sys_call_handler)do_close,
	/* #8 */ (sys_call_handler)do_read,
	/* #9 */ (sys_call_handler)do_write
};


int sys_call_max = sizeof(sys_call_table)/sizeof(sys_call_handler);




