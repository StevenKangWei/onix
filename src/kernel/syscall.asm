[bits 32]

[section .text]

global pause
pause:
    hlt
    ret

global halt
halt:
    hlt
    jmp halt

_NR_get_ticks       equ 0
INT_VECTOR_SYS_CALL equ 0x90

global get_ticks
get_ticks:
    mov eax, _NR_get_ticks
    int INT_VECTOR_SYS_CALL
    ret
