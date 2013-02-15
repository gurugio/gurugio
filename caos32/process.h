
#ifndef	__PROCESS_H__
#define	__PROCESS_H__

#include "processor.h"
#include "memory.h"


/* declare process's statements... */
#define	PROCESS_RUNNING				1
#define	PROCESS_INTERRUPTIBLE		2
#define	PROCESS_UNINTERRUPTIBLE		3
#define	PROCESS_STOPPED				4

/* maximum task number */
#define MAX_TASK 5

/* Each task has a kernel-mode stack of one page size */
#define	PROCESS_STACK_SIZE	PAGE_SIZE

/* INIT task PID is 0 */
#define	INIT_PID	0

/* address for init task */
#define TSS_ADDR			0xc0002000

// key buffer size
#define KEY_INPUT_MAX 32

/* process table... */
typedef struct _task_struct
{
	int	status;
	int	pid;
	char name[16];
	char key_input[KEY_INPUT_MAX];
	int key_offset;
	
	unsigned int priority;
	unsigned int counter;

	unsigned int sleep;

	unsigned int task_addr;

	unsigned long cr3;

	thread_struct thread;
	
} task_struct;



/*
 * Process Control Block
 * It maintains all of information about user process
 * & kernel mode stack
 */
typedef union _task_union
{
	task_struct	task;

	unsigned char	stack[ PROCESS_STACK_SIZE ];
} task_union;




void start_init(void);
void init_process(void);

void setup_task(void);
int add_task(task_struct *new);
int del_task(int);

void init_cpu_tss(void);
int create_task(void (*)(void), char *);

void set_foreground(task_struct *);


extern task_struct *fore_task;

#endif
