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

    FAT12_READ_FILE

    jmp $

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
    jmp $

read_not_found:
    mov si, MESSAGE_RETURN
    call bios_print
    mov si, MESSAGE_NOFOUND
    call bios_print
    jmp $

BIOS_PRINT_FUNCTION

MEMORY_SEGMENT  equ KERNEL_SEGMENT
MEMORY_OFFSET   equ KERNEL_OFFSET

FILENAME db "KERNEL  BIN", 0
MESSAGE_RETURN db 13, 10, 0
MESSAGE_LOADING db "Onix OS is Loading...", 0
MESSAGE_FAILURE db "Failure to read kernel...", 0
MESSAGE_SUCCESS db "Success to read kernel...", 0
MESSAGE_NOFOUND db "Kernel is not found...", 0
MESSAGE_FOUND db "Loading kernel from disk...", 0
