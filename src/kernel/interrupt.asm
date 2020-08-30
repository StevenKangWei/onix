[section .text]
%include "onix/const.inc"

extern hwint_test_handler

global hwint00
global hwint01
global hwint02
global hwint03
global hwint04
global hwint05
global hwint06
global hwint07
global hwint08
global hwint09
global hwint10
global hwint11
global hwint12
global hwint13
global hwint14
global hwint15

%macro hwint_master    1
    push %1
    call hwint_test_handler
    add esp, 4
    iretd
%endmacro

ALIGN   16
hwint00:    ; Interrupt routine for irq 0 (the clock).
    inc byte[gs:0]
    mov al, EOI
    out INT_M_CTL, al
    iretd

ALIGN   16
hwint01:    ; Interrupt routine for irq 1 (keyboard)
    hwint_master 1

ALIGN   16
hwint02:    ; Interrupt routine for irq 2 (cascade!)
    hwint_master 2

ALIGN   16
hwint03:    ; Interrupt routine for irq 3 (second serial)
    hwint_master 3

ALIGN   16
hwint04:    ; Interrupt routine for irq 4 (first serial)
    hwint_master 4

ALIGN   16
hwint05:    ; Interrupt routine for irq 5 (XT winchester)
    hwint_master 5

ALIGN 16
hwint06:    ; Interrupt routine for irq 6 (floppy)
    hwint_master 6

ALIGN 16
hwint07:    ; Interrupt routine for irq 7 (printer)
    hwint_master 7

%macro  hwint_slave 1
    push %1
    call hwint_test_handler
    add esp, 4
    iretd
%endmacro

ALIGN   16
hwint08:    ; Interrupt routine for irq 8 (realtime clock).
    hwint_slave 8

ALIGN   16
hwint09:    ; Interrupt routine for irq 9 (irq 2 redirected)
    hwint_slave 9

ALIGN   16
hwint10:    ; Interrupt routine for irq 10
    hwint_slave 10

ALIGN   16
hwint11:    ; Interrupt routine for irq 11
    hwint_slave 11

ALIGN   16
hwint12:    ; Interrupt routine for irq 12
    hwint_slave 12

ALIGN   16
hwint13:    ; Interrupt routine for irq 13 (FPU exception)
    hwint_slave 13

ALIGN   16
hwint14:    ; Interrupt routine for irq 14 (AT winchester)
    hwint_slave 14

ALIGN   16
hwint15:    ; Interrupt routine for irq 15
    hwint_slave 15


extern tss
extern process_ready

global _restart
_restart:

    mov esp, [process_ready]
    lldt [esp + LDT_SELECTOR]
    lea eax, [esp + PROCESS_STACK_TOP]
    mov dword [tss + TSS3_S_SP0], eax

    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 4

    retd
