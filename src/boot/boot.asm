[org 0x7c00]

mov ax, cs
mov ds, ax
mov ss, ax
mov si, MESSAGE_HELLO
call print 
jmp $

print:
    cld
print_loop:
    lodsb
    or al, al
    jz print_done
    mov ah, 0x0E
    int 0x10
    jmp print_loop
print_done:
    ret

MESSAGE_HELLO db "Hello, Onix!!!", 0

times 510-($-$$) db 0
dw 0xaa55
