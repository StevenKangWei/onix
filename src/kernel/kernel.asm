%include "onix/const.inc"

KERNEL_CODE_SEGMENT equ 8
bits 32
[SECTION .bss]
StackSpace		resb	2 * 1024
StackTop:		; 栈顶

[section .text]
extern KERNEL_STACK_TOP
extern init_kernel
extern enter_kernel

global _start

_start:
    ; mov esp, [KERNEL_STACK_TOP]
    mov esp, StackTop
    call init_kernel
    jmp KERNEL_CODE_SEGMENT:_entry
_entry:
    call enter_kernel
    jmp $
