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
