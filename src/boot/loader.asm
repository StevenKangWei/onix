[org 0x100]

%include "bios.inc"
%include "fat12.inc"

FAT12_HEADER _start

BASE_STACK equ 0x100

_start:
    BIOS_SETUP_STACK BASE_STACK
    BIOS_CLEAR_SCREEN

    mov si, MESSAGE_LOADING
    call bios_print

    BIOS_RESET_FLOPPY

    jmp $

BIOS_PRINT_FUNCTION

MESSAGE_LOADING:
    db "Loading...", 0
