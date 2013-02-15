#include "sched.h"

#include "process.h"
#include "console.h"


//#define DEBUG 1


/* global variables must be changed to 
 * parameters of schedule() function */
extern tss_struct *cpu_tss;
extern task_struct *task_table[MAX_TASK];
extern unsigned int task_count;


int cur_task;
unsigned int sched_count = 0;


int schedule_enable = 0;



void init_scheduler(void)
{
	cur_task = 0;
	sched_count = 0;
}


void start_scheduler(void)
{
	schedule_enable = 1;
	caos_printf("scheduler start!\n");
}


void stop_scheduler(void)
{
	schedule_enable = 0;
	caos_printf("scheduler stop!\n");
}

void schedule(void)
{

	task_struct *prev, *next;
	unsigned int *p_ebp;
	unsigned int *prev_regs;
	unsigned int *regs_in_stack;
	int l;
	unsigned int next_regs;
#ifdef DEBUG
	thread_struct *temp_thread;
#endif

	asm volatile (
			"mov %%ebp, %0	\n\t"
			: "=m"(p_ebp)
	);
	
	// 
	// ebp+4*15 = CS
	//
	if ( (p_ebp[15] & 0x3) == 0)
		return;


	// choose next task
#ifdef DEBUG
	caos_printf("cur=%d, ", cur_task);
#endif
	prev = task_table[cur_task];
	//if (cur_task == MAX_TASK-1) 
	if (cur_task == task_count-1)
		cur_task = 0;
	else 
		cur_task++;
	next = task_table[cur_task];
#ifdef DEBUG
	caos_printf("next=%d\n", cur_task);
#endif

	prev->status = PROCESS_INTERRUPTIBLE;
	next->status = PROCESS_RUNNING;


	//
	// ebp -> ebp original value
	//        return address of schedule
	// ebp+4*2 -> starting point of context
	regs_in_stack = &(p_ebp[2]);

	prev_regs = (unsigned int *)&(prev->thread);

	//
	// copy register values in stack to previous process context
#ifdef DEBUG
	//caos_printf("REGS in stack(%x)", (unsigned int)p_ebp);
#endif
	for (l=0; l<17; l++) {
		prev_regs[l] = regs_in_stack[l];
#ifdef DEBUG
	//	caos_printf("[%d]%x ", l, regs_in_stack[l]);
#endif
	}
#ifdef DEBUG
	//caos_printf("\n");
#endif

	next_regs = (unsigned int)&(next->thread);

#ifdef DEBUG
	caos_printf("prev PCB [%x] : ", (unsigned int)prev);
	caos_printf("%x %x %x %x %x\n", prev->thread.eip,
								prev->thread.cs, prev->thread.eflags,
								prev->thread.esp, prev->thread.ss);


	caos_printf("next pcb [%x] : ", (unsigned int)next_regs);
	temp_thread = (thread_struct *)next_regs;

	caos_printf("%x %x %x %x %x\n", temp_thread->eip,
								temp_thread->cs, temp_thread->eflags,
								temp_thread->esp, temp_thread->ss);
#endif

	sched_count++;

	asm volatile (
		
		"mov %5, %%eax		\n\t"
		"mov %%eax, %2		\n\t"
		"mov %%eax, %%cr3	\n\t"

		"mov %3, %%eax		\n\t"		// change kernel mode stack
		"mov %%eax, %1		\n\t"

		//
		// Pointer of next task must be stored
		// before stack is changed!!
		// If stack pointer is changed,
		// no local variables can be accessed.
		// But global variables is not refered with stack pointer.
		//
		"mov %4, %%eax		\n\t"

		"mov %%ebp, %%esp	\n\t"
		"pop %%ebp			\n\t"
		"add $4, %%esp		\n\t"		// pop return address
		"add $68, %%esp		\n\t"		// pop context of prev task
		"mov %%esp, %0		\n\t"		// store prev stack-pointer

		"mov %%eax, %%esp		\n\t"		// extract context of next task
		"popal				\n\t"
		"pop %%ds		\n\t"
		"pop %%es		\n\t"
		"pop %%fs		\n\t"
		"pop %%gs		\n\t"
		"iret				\n\t"
		: "=m"(prev->thread.esp0),"=m"(cpu_tss->esp0), "=m"(cpu_tss->cr3)
		: "m"(next->thread.esp0),"m"(next_regs),"m"(next->cr3)
	);
	return;
}
	
