%include "onix/const.inc"

KERNEL_CODE_SEGMENT equ 8
bits 32
[SECTION .bss]

[section .text]
extern KERNEL_STACK_TOP
extern init_kernel
extern enter_kernel

global _start

_start:
    mov esp, [KERNEL_STACK_TOP]
    call init_kernel
    jmp KERNEL_CODE_SEGMENT:_entry
_entry:
    jmp enter_kernel
    jmp $
