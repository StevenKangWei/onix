[bits 32]

[section .text]

global pause
pause:
    sti
    hlt
    ret

global halt
halt:
    hlt
    jmp halt

_NR_get_ticks   equ 0
_NR_write       equ 1

INT_VECTOR_SYS_CALL equ 0x90

global get_ticks
get_ticks:
    mov eax, _NR_get_ticks
    int INT_VECTOR_SYS_CALL
    ret

global write
write:
    mov eax, _NR_write
    mov ebx, [esp + 4]
    mov ecx, [esp + 8]
    int INT_VECTOR_SYS_CALL
    ret