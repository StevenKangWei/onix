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
    mov eax, SYSCALL_INDEX_PAUSE
    int INT_VECTOR_SYS_CALL
    ret

global get_ticks
get_ticks:
    mov eax, SYSCALL_INDEX_GET_TICKS
    int INT_VECTOR_SYS_CALL
    ret

global _sendrecv
_sendrecv:
    mov eax, SYSCALL_INDEX_SENDRECV
    mov ebx, [esp + 4] ; type send or recv
    mov ecx, [esp + 8] ; peer
    mov edx, [esp + 12] ; message
    int INT_VECTOR_SYS_CALL
    ret

extern syscall_table
extern save_context
extern process_ready

global _syscall
_syscall:
    call save_context

    sti
    push esi

    push dword [process_ready]
    push edx
    push ecx
    push ebx

    call [syscall_table + eax * 4]
    add esp, 4 * 4

    pop esi
    mov [esi + EAXREG - PROCESS_STACKBASE], eax
    cli

    ret