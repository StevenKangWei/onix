KERNEL_STACK_SIZE equ 4096

[section .bss]
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE

[section .text]

extern __enter_kernel
extern __init_kernel

global _start

_start:
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    call __init_kernel
    jmp $
