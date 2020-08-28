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

    BIOS_CLEAR_SCREEN
    BIOS_PRINT MESSAGE_ENTER_PROTECT_MODE

    cli ; no interrupt
    push ds; save real mode

    lgdt [GDTPTR]; load gdt register

    in al, 92h
    or al, 2
    out 92h, al

    mov eax, cr0    ; switch to protected mode
    or al, 1        ; set protect mode bit
    mov cr0, eax

    ; jump to protect mode
    jmp dword SelectorCode:( LOADER_ADDRESS + _begin_protect_mode)
    jmp $ ; should never go here

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
MESSAGE_ENTER_REAL_MODE     db "Kernel is entering to real mode...", 0

%include "mode.inc"

BASE_GDT:
    Descriptor 0,       0,          0; empty
DESC_CODE:
    Descriptor 0,       0xfffff,    DA_CR  | DA_32 | DA_LIMIT_4K
DESC_DATA:
    Descriptor 0,       0xfffff,    DA_DRW | DA_32 | DA_LIMIT_4K
DESC_VIDEO:
    Descriptor 0xB8000, 0ffffh,     DA_DRW | DA_DPL3

GDTLEN  equ $ - BASE_GDT
GDTPTR  dw GDTLEN - 1
        dd LOADER_ADDRESS + BASE_GDT

SelectorCode    equ DESC_CODE    - BASE_GDT
SelectorData    equ DESC_DATA    - BASE_GDT
SelectorVideo   equ DESC_VIDEO   - BASE_GDT | SA_RPL3

[SECTION .s32]

ALIGN 32

[BITS 32]

_begin_protect_mode:
    mov ax, SelectorVideo
    mov gs, ax
    mov ax, SelectorData
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov esp, STACK_TOP

    call init_kernel

    jmp SelectorCode: KERNEL_ENTRY
    jmp $

memcpy:
    push ebp
    mov ebp, esp

    push esi
    push edi
    push ecx

    mov edi, [ebp + 8] ; Destination
    mov esi, [ebp + 12] ; Source
    mov ecx, [ebp + 16] ; Counter
.1:
    cmp ecx, 0
    jz .2

    mov al, [ds:esi]
    inc esi
    mov byte [es:edi], al
    inc edi

    dec ecx
    jmp .1
.2:
    mov eax, [ebp + 8]
    pop ecx
    pop edi
    pop esi
    mov esp, ebp
    pop ebp
    ret

init_kernel:
    xor esi, esi
    mov cx, word [KERNEL_ADDRESS + 2Ch];
    movzx ecx, cx
    mov esi, [KERNEL_ADDRESS + 1Ch]
    add esi, KERNEL_ADDRESS
.begin:
    mov eax, [esi + 0]
    cmp eax, 0
    jz .no_action
    push dword [esi + 010h]
    mov eax, [esi + 04h]
    add eax, KERNEL_ADDRESS
    push eax
    push dword [esi + 08h]
    call memcpy
    add esp, 12
.no_action:
    add esi, 020h
    dec ecx
    jnz .begin
    ret


[SECTION .data1]

STACK_SPACE: times 0x1000 db 0
STACK_TOP equ LOADER_ADDRESS + $
