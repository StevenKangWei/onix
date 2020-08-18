%ifndef ONIX_BIOS_INC
%define ONIX_BIOS_INC

%macro BIOS_SETUP_STACK 1
    mov ax, cs
    mov ds, ax
    mov ss, ax
    mov sp, %1
%endmacro

%macro BIOS_CLEAR_SCREEN 0
    mov ax, 0x0003
    int 0x10
%endmacro

%macro BIOS_RESET_FLOPPY 0
    xor ah, ah
    xor dl, dl
    int 0x13
%endmacro

%macro BIOS_CLOSE_FLOPPY_MOTOR 0
    push dx
    mov dx, 0x03f2
    mov al, 0
    out dx, al
    pop dx
%endmacro

%macro BIOS_PRINT_FUNCTION 0
%ifndef BIOS_PRINT_FUNCTION
%define BIOS_PRINT_FUNCTION
bios_print:
    cld
bios_print_loop:
    lodsb
    or al, al
    jz bios_print_done
    mov ah, 0x0E
    int 0x10
    jmp bios_print_loop
bios_print_done:
    ret
%endif
%endmacro

%macro BIOS_PRINT 1
mov si, %1
call bios_print
%endmacro

%macro BIOS_LN_PRINT 1
mov si, MESSAGE_RETURN
call bios_print
mov si, %1
call bios_print
%endmacro

%endif