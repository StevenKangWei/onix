[bits 32]

[section .text]

global write_byte
write_byte: ; void write_byte(int addr, int data); sys.h
    mov ecx, [esp + 4] ; addr
    mov al, [esp + 8] ; data
    mov [ecx], al
    ret

global read_byte
read_byte:
    mov ecx, [esp + 4]; address
    mov eax, [ecx]
    ret

global io_outb
io_outb:
    mov edx, [esp + 4] ; port
    mov al, [esp + 8] ; value
    out dx, al
    nop ; delay
    nop
    nop
    ret

global io_inb
io_inb:
    mov edx, [esp + 4] ; port
    xor eax, eax
    in al, dx
    nop ; delay
    nop
    nop
    ret

global io_sti
io_sti:
    sti
    ret

global io_cli
io_cli:
    cli
    ret