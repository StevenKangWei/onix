[org 0x100]

%include "bios.inc"
%include "fat12.inc"

FAT12_HEADER _start

BASE_STACK equ 0x100

_start:
    BIOS_SETUP_STACK BASE_STACK
    BIOS_CLEAR_SCREEN

    BIOS_PRINT MESSAGE_LOADING

    FAT12_READ_FILE

    jmp $

read_failure:
    BIOS_LN_PRINT MESSAGE_FAILURE
    jmp $

read_not_found:
    BIOS_LN_PRINT MESSAGE_NOFOUND
    jmp $

read_success:
    BIOS_LN_PRINT MESSAGE_SUCCESS
    BIOS_CLOSE_FLOPPY_MOTOR
    jmp $

BIOS_PRINT_FUNCTION

MEMORY_SEGMENT  equ KERNEL_SEGMENT
MEMORY_OFFSET   equ KERNEL_OFFSET

FILENAME db "KERNEL  BIN", 0
FILESIZE dd 0
MESSAGE_RETURN              db 13, 10, 0
MESSAGE_LOADING             db "Onix OS is Loading...", 0
MESSAGE_FAILURE             db "Failure to read kernel...", 0
MESSAGE_SUCCESS             db "Success to read kernel...", 0
MESSAGE_NOFOUND             db "Kernel is not found...", 0
MESSAGE_FOUND               db "Loading kernel from disk...", 0
MESSAGE_ENTER_PROTECT_MODE  db "Kernel is entering protected mode...", 0
MESSAGE_ENTER_REAL_MODE     db "Kernel is back to real mode...", 0
