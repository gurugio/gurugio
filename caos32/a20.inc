;****************************************************************
; Enable A20.  This is at the very best an annoying procedure.
; A20 code ported from SYSLINUX 1.52-1.63 by H. Peter Anvin.
; AMD Elan bug fix by Robert Schwebel.
;****************************************************************

;****************************************************************
;CPU�� AMD ELAN �� ��� 
;
;mov al, 0x02                 ; �� �ϳ��� A20 ����Ʈ
;out 0x92, al                 ; �̰��� SC410/SC520���� �����Ѵ�.
;a20_elan_wait:
;call a20_test
;jz a20_elan_wait
;jmp a20_done
;
;*****************************************************************

A20_TEST_LOOPS          equ  32           ; wait �� �������� �ݺ� ��
A20_ENABLE_LOOPS        equ 255           ; ������ loop �� ��
A20_TEST_ADDR           equ 4*0x80


a20_try_loop:

; ����, ��ǻ�Ϳ� A20 ����Ʈ�� ������ �˾ƺ���.
a20_none:
call    a20_test
jnz     a20_done

; ������,  BIOS (INT 0x15, AX=0x2401)�� ������ ����.
a20_bios:
mov     ax, 0x2401 
pushfd                                  ; �� ����ġ�� FLAG�� �ǽ��Ѵ�. 
int     0x15
popfd

call    a20_test
jnz     a20_done

; Ű���� ��Ʈ�ѷ��� ���� A20 ����Ʈ�� �Ѻ���.
a20_kbc:
call    empty_8042

call    a20_test                        ; BIOS ������� ������ �Ǿ��ٸ� 
jnz     a20_done                        ; delay �� ����

mov     al, 0xD1                      ; command�� ����Ʈ�Ѵ�.
out     0x64, al
call    empty_8042

mov     al, 0xDF                      ; A20 on
out    0x60, al
call    empty_8042

; A20�� ��¥�� ���� �� ���� ��ٸ���. �̰��� � �ý��ۿ�����
; ����� ���� �ð��� �ɸ� ���� �ִ�.
; Toshiba Tecras�� �̷��� ������ ������ �ִٰ� ��������.
a20_kbc_wait:
xor     cx, cx
a20_kbc_wait_loop:
call    a20_test
jnz     a20_done
loop    a20_kbc_wait_loop

; ������ �õ�: "������Ʈ A"
a20_fast:
in      al, 0x92                      ; ������Ʈ A
or      al, 0x02                      ; "fast A20" ������
and     al, 0xFE                      ; ���ڱ� RESET���� �ʴ´�.
out     0x92, al

; ������Ʈ A�� ȿ���� ��Ÿ���⸦ ��ٸ���.
a20_fast_wait:
xor    cx, cx
a20_fast_wait_loop:
call    a20_test
jnz     a20_done
loop    a20_fast_wait_loop

; A20�� ������ ������ ������ �ʴ´�. �ٽ� �� ����. 
; 
dec     byte [a20_tries]
jnz     a20_try_loop

a20_die:
hlt
jmp     a20_die

a20_tries:
db   A20_ENABLE_LOOPS

; ������� �´ٸ�, A20�� ���� ���̴�.
a20_done:
ret

a20_test:
push    cx
push    ax
xor     cx, cx
mov     fs, cx                        ; Low memory
dec     cx
mov     gs, cx                        ; High memory area
mov     cx, A20_TEST_LOOPS
mov     ax, word [fs:A20_TEST_ADDR]
push    ax
a20_test_wait:
inc     ax
mov     word [fs:A20_TEST_ADDR], ax
call    delay                           
cmp     ax, word [gs:A20_TEST_ADDR+0x10]
loop   a20_test_wait

pop     word [fs:A20_TEST_ADDR]
pop     ax
pop     cx
ret     


empty_8042:
push   ecx
mov    ecx, 100000

empty_8042_loop:
dec    ecx
jz     empty_8042_end_loop

call   delay

in     al, 0x64                      ; 8042 ������Ʈ 
test   al, 1                         ; ��� ���� �׽�Ʈ
jz     no_output

call   delay
in    al, 0x60                      ; �д´�.
jmp    empty_8042_loop

no_output:
test   al, 2                         ; �Է� ���۰� ����á�°�? 
jnz    empty_8042_loop               ; yes - loop�� ����.

empty_8042_end_loop:
pop    ecx
ret

delay:
out    0x80, al
ret
