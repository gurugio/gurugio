bits 32

%include "selector.inc"

extern caos_printf

extern do_divide_error	; 0
extern do_debug			;1
extern do_nmi				;2
extern do_int3			;3
extern do_overflow		;4
extern do_bounds			;5
extern do_invalid_op		;6
extern do_device_not_available	;7
extern do_doublefault_fn			;8
extern do_coprocessor_segment_overrun		;9
extern do_invalid_TSS		;10
extern do_segment_not_present	;11
extern do_stack_segment	;12
extern do_general_protection	;13
extern do_page_fault		;14
extern do_coprocessor_error	;16
extern do_alignment_check		;17
extern do_machine_check		;18
extern do_simd_coprocessor_error	;19




; [esp+0] - ebx
; [esp+4] - ecx
; ...

_EBX_			equ 0x0
_ECX_			equ 0x4
_EDX_			equ 0x8
_ESI_			equ 0xc
_EDI_			equ 0x10
_EBP_			equ 0x14
_EAX_			equ 0x18
_DS_			equ 0x1c
_ES_			equ 0x20
_HANDLER_		equ 0x24
_ERROR_			equ 0x28

_EIP_			equ 0x2c
_CS_			equ 0x30
_EFLAGS_		equ 0x34
_OLD_ESP_		equ 0x38
_OLD_SS_		equ 0x3c

; save all register, it save context of current process.
;%macro	SAVE_ALL 0
;	push gs
;	push fs
;	push es
;	push ds
;	pushad
;	pushfd

;	; set KERNEL CODE segment
;	mov ax, 0x10
;	mov ds, ax
;	mov es, ax
;	mov fs, ax
;	mov gs, ax
;%endmacro

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


; restore all register, it restore context of previous process
;%macro	RESTORE_ALL 0
;	popfd
;	popad
;	pop ds
;	pop es
;	pop fs
;	pop gs
;%endmacro

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


	global _test_except
	global _ignore_isr
	global _lidt

	global _divide_error	; 0
	global _debug			;1
	global _nmi				;2
	global _int3			;3
	global _overflow		;4
	global _bounds			;5
	global _invalid_op		;6
	global _device_not_available	;7
	global _doublefault_fn			;8
	global _coprocessor_segment_overrun		;9
	global _invalid_TSS		;10
	global _segment_not_present	;11
	global _stack_segment	;12
	global _general_protection	;13
	global _page_fault		;14
	; 15th exception is reserved for INTEL
	global _coprocessor_error	;16
	global _alignment_check		;17
	global _machine_check		;18
	global _simd_coprocessor_error	;19
	;global _system_call			;128


_test_except:

	pushad

	
	int 0x1
	int 0x0
	int 0x2
	int 0x3
	int 0x4
	int 0x5
	int 0x7

	int 128
	int 0x25	

	popad
	ret

	;------------------------------
	; enable IDT
	;------------------------------
	align 4
_lidt:
	pushad

	lidt [idtr]

	popad
	ret

	align 4
idtr:
	dw 0x1000
	dd 0xC0000000	

	align 4
_error_code:
	SAVE_ALL
	
	mov edi, [esp+_HANDLER_] 
	mov eax, [esp+_ERROR_]
	
	push eax
	call edi
	add esp, 4

	jmp _ret_from_exception

	align 4
_ret_from_exception:
	RESTORE_ALL

	add esp, 8

	;== no exception processing yet
	jmp $

	iret

	;-----------------------------
	; Common ISR for unused interrupt
	;-----------------------------
	align 4
_ignore_isr:
	SAVE_ALL

	push int_err
	call caos_printf
	add esp, 4

	RESTORE_ALL

	iret


	;---------------------------------------
	; Exception handlers
	;---------------------------------------
	align 4
_divide_error:
	push int0_msg
	call caos_printf
	add esp, 4

	push 0		; no error code
	push do_divide_error
	jmp _error_code


	align 4
_debug:
	push int1_msg
	call caos_printf
	add esp, 4

	push 0
	push do_debug
	jmp _error_code


	align 4
_nmi:
	push int2_msg
	call caos_printf
	add esp, 4

	push 0
	push do_nmi
	jmp _error_code


	align 4
_int3:
	push int3_msg
	call caos_printf
	add esp, 4

	push 0
	push do_int3
	jmp _error_code


	align 4
_overflow:
	push int4_msg
	call caos_printf
	add esp, 4

	push 0
	push do_overflow
	jmp _error_code


	align 4
_bounds:
	push int5_msg
	call caos_printf
	add esp, 4

	push 0
	push do_bounds
	jmp _error_code


	align 4
_invalid_op:
	push int6_msg
	call caos_printf
	add esp, 4

	push 0
	push do_invalid_op
	jmp _error_code


	align 4
_device_not_available:
	push int7_msg
	call caos_printf
	add esp, 4

	push 0
	push do_device_not_available
	jmp _error_code



	align 4
_doublefault_fn:
	push int8_msg
	call caos_printf
	add esp, 4

	push do_doublefault_fn
	jmp _error_code



	align 4
_coprocessor_segment_overrun:
	push int9_msg
	call caos_printf
	add esp, 4

	push 0
	push do_coprocessor_segment_overrun
	jmp _error_code



	align 4
_invalid_TSS:
	push int10_msg
	call caos_printf
	add esp, 4

	push do_invalid_TSS
	jmp _error_code


	align 4
_segment_not_present:
	push int11_msg
	call caos_printf
	add esp, 4

	push do_segment_not_present
	jmp _error_code


	align 4
_stack_segment:
	push int12_msg
	call caos_printf
	add esp, 4

	push do_stack_segment
	jmp _error_code


	align 4
_general_protection:
	push int13_msg
	call caos_printf
	add esp, 4

	push do_general_protection
	jmp _error_code



	align 4
_page_fault:
	push int14_msg
	call caos_printf
	add esp, 4

	push do_page_fault
	jmp _error_code


	align 4
_coprocessor_error:
	push int16_msg
	call caos_printf
	add esp, 4

	push 0
	push do_coprocessor_error
	jmp _error_code


	align 4
_alignment_check:
	push int17_msg
	call caos_printf
	add esp, 4

	push do_alignment_check
	jmp _error_code


	align 4
_machine_check:
	push int18_msg
	call caos_printf
	add esp, 4

	push 0
	push do_machine_check
	jmp _error_code


	align 4
_simd_coprocessor_error:
	push int19_msg
	call caos_printf
	add esp, 4

	push 0
	push do_simd_coprocessor_error
	jmp _error_code



;	align 4
;_system_call:
;	SAVE_ALL

;	push int128_msg
;	call caos_printf
;	add esp, 4


	; system call exception will be implemented later...

;	RESTORE_ALL
;	iret




	
	align 4
int_err db "Unknown Interrupt", 0
int0_msg db "Divide Error", 0
int1_msg db "Debug", 0
int2_msg db "Non-Maskable Interrupt", 0
int3_msg db "Breakpoint", 0
int4_msg db "Overflow", 0
int5_msg db "Bound check", 0
int6_msg db "Invalid Opcode", 0
int7_msg db "Device not Available", 0
int8_msg db "Double Fault", 0
int9_msg db "Coprocessor Segment Overrun", 0
int10_msg db "Invalid TSS", 0
int11_msg db "Segment not Present", 0
int12_msg db "Stack Segment", 0
int13_msg db "General Protection", 0
int14_msg db "Page Fault", 0
int16_msg db "Floating Point Error", 0
int17_msg db "Alignment Check", 0
int18_msg db "Machine Check", 0
int19_msg db "SIMD Error", 0
;int128_msg db "System Call", 0


