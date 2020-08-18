[section .text]

extern __kernel_entry
global _start

_start:
    call __kernel_entry
    jmp $

global _print_kernel
_print_kernel:
    mov ah, 0Fh;
    mov al, 'K'
    mov [gs:0], ax;
    ret
