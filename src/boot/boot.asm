[org 0x7c00]

%include "bios.inc"
%include "fat12.inc"

FAT12_HEADER _start

BASE_STACK equ 0x7c00

_start:
    BIOS_SETUP_STACK BASE_STACK
    BIOS_CLEAR_SCREEN

    mov si, MESSAGE_LOADING
    call bios_print

    BIOS_RESET_FLOPPY

    jmp $

read_sector:
    ret


BIOS_PRINT_FUNCTION

MESSAGE_LOADING:
    db "Booting...", 0

times 510-($-$$) db 0
dw 0xaa55
