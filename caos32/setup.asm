%include "selector.inc"


swapper_pg_dir	equ 0x3000
pg0				equ 0x4000

GDT_ADDR		equ 0x1000
TSS_ADDR		equ 0x2000

KERNEL_REAL_ADDR	equ 0xA000
KERNEL_VIRT_ADDR	equ 0x100000


KSECT equ KSIZE


[bits 16]
[org 0x9000]

setup_start:

	mov ax, cs		; cs=ds=es=0x0
	mov ds, ax
	mov es, ax

	mov ax, 0xb800
	mov es, ax
	mov bx, 0x2
	mov al, byte [setup_msg]
	mov byte [es:bx], al
	
	; clear TSS
	; 0x6000 ~ 0x6FFF is allocated for TSS
	mov edi, TSS_ADDR
	xor eax, eax
	mov ecx, 26
	cld
	rep stosd

	;
	; make TSS segment descriptor
	;
	xor eax, eax
	mov eax, TSS_ADDR
	mov [descriptor4+2], ax
	shr eax, 16
	mov [descriptor4+4], al
	mov [descriptor4+7], ah


	lea si, [gdt]	; source : ds:si=0x0:gdt
	xor ax, ax
	mov es, ax		; destination : es:di=0x0:0x1000
	mov di, GDT_ADDR
	mov cx, 8*7	; 7 descriptors

	rep movsb

	cli
	lgdt [gdtr]


	mov eax, cr0
	or eax, 0x0000001
	mov cr0, eax

	jmp $+2
	nop
	nop

	jmp dword KERNEL_CODE_SEL:protect_start


[bits 32]
protect_start:
	mov ax, KERNEL_DATA_SEL
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov edi, 0xb8004
	mov al, byte [protect_msg]
	mov byte [edi], al


	; check physical memory size
	mov esi, 0x100000
	mov eax, 0x1234abcd
	mov ecx, 0
check_mem_size:
	inc ecx
	mov dword [ds:esi], eax
	mov ebx, dword [ds:esi]
	add esi, 0x100000
	cmp eax, ebx
	je check_mem_size
	mov [memory_size], ecx



	; temporary stack for booting process
	mov esp, 0x9ffff

	;
	; kernel exists at 0x100000~
	;
	mov ax, KSECT
	mov dx, 512		; sector = 512byte
	mul dx
	mov cx, ax

	mov edx, 0

	mov esi, KERNEL_REAL_ADDR
	mov edi, KERNEL_VIRT_ADDR
	
copy_kernel:
	inc edx
	mov al, byte [ds:esi]
	mov byte [es:edi], al
	inc esi
	inc edi
	dec cx
	jnz copy_kernel	

	mov [kernel_size], edx

	; set PAGE DIRECTORY

	mov edi, swapper_pg_dir+0x0*4		; 0th table <- 0x3000
	mov eax, pg0
	or eax, 0x7			; 0~4MB is not accesable to user task
	mov [es:edi], eax

	mov edi, swapper_pg_dir+0x300*4	; 0x300th table <- 0x3000
	mov eax, pg0
	or eax, 0x3
	mov [es:edi], eax

	;
	; set PAGE TABLE for global tables
	; physical memory 0x0~0x400000 is mapped to 
	; virtual memory 0x0~0x400000(4MB) & 0xC0000000 ~ 4MB
	;
	mov edi, pg0		; first kernel page table at 0x4000
	mov eax, 0x00000007
	mov ecx, 1024
set_tbl_table:
	mov [es:edi], eax
	add eax, 0x1000
	add edi, 4
	dec ecx
	jnz set_tbl_table


	mov eax, swapper_pg_dir
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	; stack space is grow down from 0x9ffff
	;mov esp, 0xc009ffff

	mov edx, [kernel_size]
	mov ebx, [memory_size]
	

	jmp 0xc0100000

	jmp $

setup_msg	db "a", 0
protect_msg	db "O", 0
kernel_size dd 0
memory_size dd 0


gdtr:
	dw 0x1000
	dd 0x1000

gdt:
	dd 0x00000000, 0x00000000
	dd 0x0000FFFF, 0x00CF9A00 ; kernel code segment
	dd 0x0000FFFF, 0x00CF9200 ; kernel data segment
	dd 0x8000FFFF, 0x0040920B ; video memory segment


	;dd 0x00000000, 0x00000000	; for TSS descriptor
descriptor4:
	dw 104
	dw 0
	db 0
	db 0x89
	db 0
	db 0

	dd 0x0000FFFF, 0x00cfFA00  ; user code segment
	dd 0x0000FFFF, 0x00cfF200  ; user data segment
	dd 0x00000000, 0x00000000
gdt_end:

tss: times 104 db 0

times 2048-($-$$) db 0
