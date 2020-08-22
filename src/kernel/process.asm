%include "onix/const.inc"

[section .text]

extern tss
extern process_ready

global schedule
schedule:
    mov esp, [process_ready]
    lldt [esp + LDT_SELECTOR]
    lea eax, [esp + PROCESS_STACK_TOP]
    mov dword [tss + TSS3_S_SP0], eax

    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 4

    retd
