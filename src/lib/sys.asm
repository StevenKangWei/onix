[bits 32]

[section .text]

global pause
pause:
    hlt
    ret

global halt
halt:
    hlt
    jmp halt

global hang
hang:
    jmp short hang

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
