KERNEL_STACK_SIZE equ 4096
KERNEL_CODE_SEGMENT equ 8
[section .bss]
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE

[section .text]

extern __enter_kernel
extern __init_kernel
extern gdt_ptr

global _start

_start:
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    sgdt [gdt_ptr]
    call __init_kernel
    lgdt [gdt_ptr]
    jmp KERNEL_CODE_SEGMENT:_entry
_entry:
    call __enter_kernel
    jmp $


