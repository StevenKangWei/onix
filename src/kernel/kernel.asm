%include "onix/const.inc"

KERNEL_STACK_SIZE equ 2048
KERNEL_CODE_SEGMENT equ 8
bits 32

[section .bss]
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE

[section .text]

extern gdt_ptr
extern idt_ptr
extern tss

extern enter_kernel
extern init_kernel

global _start

_start:
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    sgdt [gdt_ptr]
    call init_kernel
    lgdt [gdt_ptr]
    lidt [idt_ptr]

    jmp KERNEL_CODE_SEGMENT:_entry
_entry:
    xor eax, eax
    mov ax, SELECTOR_TSS
    ltr ax

    ;cli;
    ;sti; allow interrupts

    call enter_kernel
    jmp $
