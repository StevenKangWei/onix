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

    FAT12_READ_FILE

read_failure:
    mov si, MESSAGE_RETURN
    call bios_print
    mov si, MESSAGE_FAILURE
    call bios_print
    jmp $

read_success:
    mov si, MESSAGE_RETURN
    call bios_print
    mov si, MESSAGE_SUCCESS
    call bios_print
    jmp MEMORY_SEGMENT:MEMORY_OFFSET

read_not_found:
    mov si, MESSAGE_RETURN
    call bios_print
    mov si, MESSAGE_NOFOUND
    call bios_print
    jmp $

BIOS_PRINT_FUNCTION

MEMORY_SEGMENT  equ LOADER_SEGMENT
MEMORY_OFFSET   equ LOADER_OFFSET

FILENAME db "LOADER  BIN", 0
MESSAGE_RETURN db 13, 10, 0
MESSAGE_LOADING db "Booting", 0
MESSAGE_FAILURE db "Failure", 0
MESSAGE_SUCCESS db "Success", 0
MESSAGE_NOFOUND db "Nofound", 0
MESSAGE_FOUND db "Found", 0

times 510-($-$$) db 0
dw 0xaa55
