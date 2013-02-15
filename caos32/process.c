
#include "process.h"
#include "console.h"
#include "segment.h"
#include "types.h"
#include "gdt.h"


#include "page.h"
#include "memory.h"


//#define DEBUG 1

//
// setup.asm에서 저장한 TSS 영역의 주소
//
tss_struct *cpu_tss;

//
// 각 태크스들의 task_struct 주소를 배열로 관리
//
task_struct *task_table[MAX_TASK];


//
// number of created tasks
unsigned int task_count;

extern pgd_t *swapper_pg_dir;


// pointer to foreground task
task_struct *fore_task;



//
// clear table entries
//
void setup_task(void)
{
	int l;
	for (l=0; l<MAX_TASK;l++) {
		task_table[l] = NULL;
	}

	task_count = 0;

}

//
// find empty entry and store new
// , return pid (process ID)
//
int add_task(task_struct *new)
{
	int l = 0;

	for (l=0; l<MAX_TASK;l++) {
		if (task_table[l] == NULL) {
			task_table[l] = new;
			return l;
		}
	}

	return -1;

}

//
// remove id(th) entry
//
int del_task(int id)
{
	if (task_table[id] == NULL)
		return -1;
	task_table[id] = NULL;
	return 1;
}


void set_foreground(task_struct *task)
{
	fore_task = task;
}


//
// func : task function pointer
// task_addr : user address space task function is copied 
// pcb_addr : address of task_struct
// name : task name
//
int create_task(void (*func)(void), char *name)
{
	int l;
	char *src;	// task function
	char *dst;	// user memory space to execute task
	task_struct *tcb;	// task control block

	pgd_t *pgd;	// page dir
	pte_t *pte;	// page table
	unsigned long addr;

#ifdef DEBUG
	caos_printf("Create task [%s]...", name);
#endif

	// 1 page memory for a page dir, a task has one page DIR
	//pgd = (pgd_t *)__pa(get_free_pages(0, 0));
	pgd = (pgd_t *)get_free_pages(0, 0);
	if (pgd == NULL)
		goto err_pgd;

#ifdef DEBUG
	caos_printf("pgd->%x ", (unsigned long)pgd);
#endif

	// 1 page memory for a page table
	// When task is created, only one table is setup.
	//pte = (pte_t *)__pa(get_free_pages(0, 0));
	pte = (pte_t *)get_free_pages(0, 0);
	if (pte == NULL)
		goto err_pte;

#ifdef DEBUG
	caos_printf("pte->%x ", (unsigned long)pte);
#endif

	// add task_struct to task table
	//tcb = (task_struct *)__pa(get_free_pages(0,0));
	tcb = (task_struct *)get_free_pages(0,0);
	if (tcb == NULL)
		goto err_tcb;

#ifdef DEBUG
	caos_printf("tcb->%x ", (unsigned long)tcb);
#endif

	// store entry point of task
	//tcb->task_addr = __pa(get_free_pages(0,0));
	tcb->task_addr = get_free_pages(0,0);
	if (tcb->task_addr == (unsigned long)NULL)
		goto err_task;

#ifdef DEBUG
	caos_printf("task->%x \n", (unsigned long)tcb->task_addr);
#endif

	tcb->pid = add_task(tcb);

	// There is no empty entry
	if (tcb->pid < 0) {
		caos_printf("Cannot create task more\n");
		goto err_task;
	}

#ifdef DEBUG
	caos_printf("PID=%d\n", tcb->pid);
#endif


	//
	// Copy function to user space so that the func executes in user mode.
	// In user mode, function in kernel space cannot run.
	//
	src = (char *)func;
	dst = (char *)tcb->task_addr;
	for (l=0 ; l < PAGE_SIZE; l++) // copy one page
		*dst++ = *src++;

	//
	// initialize TCB 
	//
	
	tcb->status = PROCESS_INTERRUPTIBLE;
	

	// build page dir/table
	// DIR=0x100, TABLE=0x0 -> 0x4000 0000
	// User task is stored at virtual address 0x4000 0000
#ifdef DEBUG
	caos_printf("setup paging..");
#endif
	tcb->cr3 = __pa((unsigned long)pgd);	// physical memory

	addr = (unsigned long)__pa(pte) + (_PAGE_RW|_PAGE_PRESENT|_PAGE_USER);
	set_pgd(pgd+0x100, __pgd(addr));	// 0x100th entry

	addr = (unsigned long)__pa(tcb->task_addr) +(_PAGE_RW|_PAGE_PRESENT|_PAGE_USER);
	set_pte(pte, __pte(addr));			// 0x0th entry
	
	//--------------------------------------------
	//
	// User task share page DIR[0],[0x300~0x3DD] with kernel,
	// so that user task can access video memory.
	// When user task is created,
	// DIR[0], [0x300~0x3ff] must be copied.
	//
	//----------------------------------------------------
	pgd[0x0].pgd = swapper_pg_dir[0x0].pgd;

	for (l=0x300; l<0x400; l++) {
		pgd[l].pgd = swapper_pg_dir[l].pgd;
	
#ifdef DEBUG
		if (pgd[l].pgd != 0)
			caos_printf("pgd[%x]=%x swapper[%x]=%x\n", l, pgd[l].pgd, l, swapper_pg_dir[l].pgd);
#endif
	}


#ifdef DEBUG
	caos_printf("CR3=%x PGD[0x100]=%x PTE[0x0]=%x\n", tcb->cr3, pgd[0x100].pgd, pte[0].pte_low);

	pte = (pte_t *)0xc0004000;
	caos_printf("kernel pte[0x36]=%x\n", pte[0x36].pte_low);
#endif

	//--------------------------------------------------
	// initialize point of task code, user-stack, kernel-stack
	// User task always have been executed at 0x40000000
	//----------------------------------------------------
	
	//tcb->thread.eip = tcb->task_addr;
	tcb->thread.eip = 0x40000000;
	//tcb->thread.esp = tcb->task_addr+PAGE_SIZE;
	tcb->thread.esp = 0x40000000+PAGE_SIZE;
	tcb->thread.esp0 = (unsigned long)tcb+PAGE_SIZE;

	tcb->thread.cs = __USER_CS;
	tcb->thread.eflags = 0x200;	// interrupt enable
	tcb->thread.ss = __USER_DS;
	tcb->thread.ds = __USER_DS;
	tcb->thread.es = __USER_DS;
	tcb->thread.fs = __USER_DS;
	tcb->thread.gs = __USER_DS;

	for (l=0; name[l] != '\0'; l++)
		tcb->name[l] = name[l];


	tcb->key_offset = 0;


	caos_printf("Create Task : %s (PID=%d) OK\n", tcb->name, tcb->pid);

	task_count++;

#ifdef DEBUG
	caos_printf("Task count=%d\n", task_count);
#endif
	return tcb->pid;

	caos_printf("fail\n");
err_task:
	free_pages((unsigned long)tcb->task_addr, 0);

err_tcb:
	free_pages((unsigned long)tcb, 0);

err_pte:
	free_pages((unsigned long)pte, 0);

err_pgd:
	free_pages((unsigned long)pgd, 0);

	return -1;

}


//
// TSS segment must be initialized to run user-mode tasks
//
void init_cpu_tss(void)
{
	
	// TSS for CPU
	cpu_tss = (tss_struct *)TSS_ADDR;


	caos_printf("CPU TSS is at->[%x]\n", (unsigned int)cpu_tss);

	// Kernel Stack segment = 0x10
	cpu_tss->ss0 = __KERNEL_DS;
	
	// TSS selector is 0x20 (5th entry in GDT)
	asm volatile (
		"cli							\n\t"
		"mov $0x20, %%ax	\n\t"
		"ltr %%ax			\n\t"
		"sti				\n\t"
		::
	);
}

//
// execute user-mode task, init
//
void start_init(void)
{	
	task_struct *init_pcb = task_table[0];
	unsigned long reg;


	set_foreground(init_pcb);

	reg = (unsigned long)&(init_pcb->thread);


	asm volatile (
		"cli							\n\t"
	
		/* 
		 * Before task start, TSS must be initialized.
		 * Therefore kernel stack pointer is store at cpu_tss->esp0.
		*/ 
		"mov %2, %%eax		\n\t"
		"mov %%eax, %0		\n\t"
		
		//
		// swapper_pg_dir is not used anymore,
		// because user task has kernel page DIR at DIR[0x300~0x3FF], 
		// swapper_pg_dir is not necessary.
		//
		"mov %4, %%eax		\n\t"
		"mov %%eax, %1		\n\t"
		
		// store user page DIR
		"mov %4, %%eax		\n\t"
		"mov %%eax, %%cr3		\n\t"

		"mov %5, %%esp		\n\t"
		"popa				\n\t"
		"pop %%ds				\n\t"
		"pop %%es				\n\t"
		"pop %%fs				\n\t"
		"pop %%gs				\n\t"
		"sti						\n\t"
		"iret						\n\t"
		: "=m"(cpu_tss->esp0), "=m"(cpu_tss->cr3)
		: "m"(init_pcb->thread.esp0), "m"(swapper_pg_dir), "m"(task_table[0]->cr3), "m"(reg)
	);
}
		

