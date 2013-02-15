bits 32

%include "selector.inc"

extern caos_printf
extern handle_irq
extern schedule
extern schedule_enable



; [esp+0] - ebx
; [esp+4] - ecx
; ...

_EDI_			equ 0x0
_ESI_			equ 0x4
_EBP_			equ 0x8
_ESP_			equ 0xc
_EBX_			equ 0x10	;16
_EDX_			equ 0x14
_ECX_			equ 0x18
_EAX_			equ 0x1c
_DS_			equ 0x20	;32
_ES_			equ 0x24
_FS_			equ 0x28
_GS_			equ 0x2c

_EIP_			equ 0x30	;48
_CS_			equ 0x34
_EFLAGS_		equ 0x38
_OLD_ESP_		equ 0x3c
_OLD_SS_		equ 0x40	;64

; SAVE_ALL is referred to linux-kernel
%macro SAVE_ALL 0
	cld

	push gs
	push fs
	push es
	push ds
	
	pushad

	mov dx, KERNEL_DATA_SEL
	mov ds, dx
	mov es, dx
%endmacro


; SAVE_ALL is referred to linux-kernel
%macro RESTORE_ALL 0
	;pop ebx
	;pop ecx
	;pop edx
	;pop esi
	;pop edi
	;pop ebp
	;pop eax

	popad

	pop ds
	pop es
	pop fs
	pop gs
%endmacro



section .text


	global _ignore_irq
	global _test_irq
	global _irq_0
	global _irq_1


	align 4
_test_irq:
	SAVE_ALL
	
	int 0x20
	int 0x21

	RESTORE_ALL
	ret


	;-----------------------------
	; Common ISR for unused interrupt
	;-----------------------------
	align 4
_ignore_irq:
	SAVE_ALL

	push irq_err
	call caos_printf
	add esp, 4

	RESTORE_ALL

	ret

irq_err db "Unkown IRQ", 0xa, 0

	;---------------------------------
	; IRQ 0 = TIMER
	;--------------------------------
	align 4
_irq_0:
	SAVE_ALL
	
	push 0		; IRQ number
	call handle_irq
	add esp, 4

	mov eax, dword [schedule_enable]
	cmp eax, 1
	jne _end_irq_0

	call schedule

_end_irq_0:
	RESTORE_ALL
	iret

timer_msg db "TIMER IRQ", 0xa, 0

	;-----------------------------------
	; IRQ 1 = KEYBOARD
	;-----------------------------------
	align 4
_irq_1:
	SAVE_ALL


	push 1		; IRQ number
	call handle_irq
	add esp, 4

	RESTORE_ALL
	iret

keybd_msg db "KEYBOARD IRQ", 0xa, 0


