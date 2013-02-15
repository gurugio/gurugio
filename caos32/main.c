#include "types.h"
#include "console.h"
#include "idt.h"
#include "inline_asm.h"
#include "keyboard.h"
#include "timer.h"
#include "gdt.h"

#include "sched.h"
#include "process.h"
#include "segment.h"

#include "user_tasks.h"
#include "init.h"
#include "memory.h"
#include "syscall.h"
#include "string.h"

#include "char_device.h"



void start_kernel(void)
{
	char str[] = "C FUNCTION START!";
		
	/*
	 * ARCHITECTURE DEPENDENT INITIALIZATIONS
	 */

	/* screen - message display */
	init_screen();
	caos_printf("%s\n", str);

	/* memory manager */
	setup_memory();

	

	/* exception & interrupt */
	init_idt();
	set_idtr();


	/* device & IRQ */
	init_char_dev();
	keyboard_init();
	init_timer();

	/* scheduling */
	init_scheduler();


	/* task management */
	setup_task();
	init_cpu_tss();


	/* after task setup, start scheduler */
	start_scheduler();



	/*************************************************/

	/*
	 * ARCHITECTURE INDEPENDENT PROCESSING
	 */


	if (create_task(init, "init") < 0)
		caos_printf("Create Init fail..\n");

	if (create_task(user1, "user1") < 0)
		caos_printf("Create user1 fail..\n");

	if (create_task(user2, "user2") < 0)
		caos_printf("Create user2 fail..\n");




	caos_printf("CaOS KERNEL END!!\n");

	/* execute user mode task */
	start_init();
	


	while (1);
}
		
		
		
