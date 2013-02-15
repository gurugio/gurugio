

bits 32

%include "selector.inc"

extern sys_call_table
extern sys_call_max

extern schedule
extern schedule_enable


; SAVE_ALL is referred to linux-kernel
%macro SAVE_ALL 0
	cld
	push es
	push ds
	push eax
	push ebp
	push edi
	push esi
	push edx
	push ecx
	push ebx
	mov dx, KERNEL_DATA_SEL
	mov ds, dx
	mov es, dx
%endmacro


; SAVE_ALL is referred to linux-kernel
%macro RESTORE_ALL 0
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	pop ebp
	pop eax
	pop ds
	pop es
%endmacro


section .text

global _system_call

	align 4

_system_call:
	;--------------------------------------
	; EAX = system call number = esp+36
	; EBX = 1st arg = esp
	; ECX = 2nd arg = esp+4
	; EDX = 3rd arg = esp+8
	; 1. System call has no argument
	;  - do not pop value from stack
	; 2. System call has one argument
	;  - C func access esp+4 as function parameter
	;  - 1st arg (EBX) is already stored in stack
	;  - Even though system call handler has parameter, 
	;    it is not necessary to push arg in stack.
	; 3. System call has 2~3 args
	;  - args are already stored in stack.
	;  - do not need to access stack.
	;---------------------------------------

	push eax	; store system call number
	SAVE_ALL

	mov edx, sys_call_table
	cmp edx, sys_call_max
	jae _end_system_call	; system call number error

	mov eax, dword [esp+36]	; read system call number
	shl eax, 2
	add edx, eax

	; args are already in stack
	call [edx]

	; store return value at stack,
	; it will be popped and store at EAX by RESTORE_ALL macro
	mov dword [esp+36], eax	; store return value at stack

	
	; if system-call takes long time,
	; scheduling takes place.
	;mov eax, dword [schedule_enable]
	;cmp eax, 1
	;jne _end_system_call
	;call schedule



_end_system_call:
	
	RESTORE_ALL
	pop eax		; store return value at EAX

	iret

syscall_msg db "system-call", 0
