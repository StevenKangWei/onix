%include "onix/const.inc"

bits 32
[section .text]

global sys_pause
sys_pause:
    sti
    hlt
    ret

global pause
pause:
    mov eax, 0
    int INT_VECTOR_SYS_CALL
    ret

global get_ticks
get_ticks:
    mov eax, 1
    int INT_VECTOR_SYS_CALL
    ret

extern syscall_table
extern save

global _syscall
_syscall:
    call save

    sti

    call [syscall_table + eax * 4]
    mov [esi + EAXREG - PROCESS_STACKBASE], eax

    cli

    ret