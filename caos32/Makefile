INCLUDE=.
CFLAGS=-Wall -I$(INCLUDE)


BOOT_SRCS = bootsect.asm setup.asm

C_SRCS = main.c kprintf.c io.c init_idt.c init_irq.c except_handler.c keyboard.c string.c timer.c sched.c process.c oops.c user_tasks.c page.c memory.c page_alloc.c init.c char_device.c syscall.c user_syscall.c
ASM_SRCS = head.asm exception.asm irq.asm syscall_handler.asm




BOOT_IMG = boot.img
C_OBJS = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.asm=.o)




MAIN_IMG = main.img


all:$(MAIN_IMG) $(BOOT_IMG)
	cat bootsect.img setup.img main.img bootsect.img > floppy.img
	nm main.tmp > text.txt

$(MAIN_IMG):$(ASM_OBJS) $(C_OBJS)
	ld -o main.tmp -Ttext 0xc0100000 $(ASM_OBJS) $(C_OBJS) 
	objcopy -j .text -j .rodata -j .data -O binary main.tmp $@


%.o : %.c
	gcc $(CFLAGS) -c $<


%.o : %.asm
	nasm -f elf -o $@ $<

$(BOOT_IMG):$(BOOT_SRCS)
	@KERNEL_SIZE=`ls -l main.img | awk '{ print $$5 }'`; \
	KERNEL_SIZE=`expr \( $$KERNEL_SIZE + 512 \) / 512`;	\
	echo "Kernel size(sectors) = " $$KERNEL_SIZE;	\
	nasm -DKERNEL_SIZE=$$KERNEL_SIZE -o bootsect.img bootsect.asm
	@KERNEL_SIZE=`ls -l main.img | awk '{ print $$5 }'`; \
	KERNEL_SIZE=`expr \( $$KERNEL_SIZE + 512 \) / 512`;	\
	echo "Kernel size(sectors) = " $$KERNEL_SIZE;	\
	nasm -DKSIZE=$$KERNEL_SIZE -o setup.img setup.asm

clean:
	rm -f *.o *.img *.tmp
