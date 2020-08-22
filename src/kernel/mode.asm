[section .text]

global load_gdt
load_gdt:
    mov eax, [esp + 4]
    mov [gdt_ptr + 2], eax
    mov ax, [esp + 8]
    mov [gdt_ptr], ax
    lgdt [gdt_ptr]
    ret
