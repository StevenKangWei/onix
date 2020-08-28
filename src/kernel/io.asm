[bits 32]

[section .text]

global out_byte
out_byte:
    mov edx, [esp + 4] ; port
    mov al, [esp + 8] ; value
    out dx, al
    nop ; delay
    nop
    nop
    ret

global in_byte
in_byte:
    mov edx, [esp + 4] ; port
    xor eax, eax
    in al, dx
    nop ; delay
    nop
    nop
    ret

global disable_interrupt
disable_interrupt:
    cli
    ret

global enable_interrupt
enable_interrupt:
    sti
    ret

