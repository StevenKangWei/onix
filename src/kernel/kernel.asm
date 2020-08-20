[section .text]

extern __enter_kernel
extern __init_kernel

global _start

_start:
    call __init_kernel
    jmp $
