KERNEL_STACK_SIZE equ 4096
KERNEL_CODE_SEGMENT equ 8
[section .bss]
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE

[section .text]

extern enter_kernel
extern init_kernel
extern gdt_ptr
extern idt_ptr

global _start

_start:
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    sgdt [gdt_ptr]
    call init_kernel
    lgdt [gdt_ptr]
    lidt [idt_ptr]
    jmp KERNEL_CODE_SEGMENT:_entry
_entry:
    sti; allow interrupts
    call enter_kernel
    jmp $


