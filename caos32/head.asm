
[bits 32]

%include "selector.inc"

extern start_kernel
extern phy_mem_size
extern kernel_size

section .text
global _start

_start:

	mov dword [kernel_size], edx
	mov dword [phy_mem_size], ebx

	mov edi, 0xC00b8006
	mov al, byte [kernel_msg]
	mov byte [edi], al
	
	; stack space is grow down from 0x9ffff
	mov esp, 0xc009fffc

	call start_kernel

	jmp $

kernel_msg	db "S", 0

