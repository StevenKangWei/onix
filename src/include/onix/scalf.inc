%ifndef ONIX_SCALF_INC
%define ONIX_SCALF_INC

[section .text]

;----------------------
dochar:
    call cprint              ; print one character
sprint:
    mov eax, [esi]          ; string char to AL
    lea esi, [esi+1]
    cmp al, 0
    jne dochar               ; else, we're done
    add byte [ypos], 1       ; down one row
    mov byte [xpos], 0       ; back to left
    ret
 
cprint:
    mov ah, 0x0F             ; attrib = white on black
    mov ecx, eax             ; save char/attribute
    movzx eax, byte [ypos]
    mov edx, 160             ; 2 bytes (char/attrib)
    mul edx                  ; for 80 columns
    movzx ebx, byte [xpos]
    shl ebx, 1               ; times 2 to skip attrib
 
    mov edi, 0xb8000         ; start of video memory
    add edi, eax             ; add y offset
    add edi, ebx             ; add x offset
 
    mov eax, ecx             ; restore char/attribute
    mov word [ds:edi], ax
    add byte [xpos], 1       ; advance to right
 
    ret
 
;------------------------------------
 
printreg32:
    mov edi, outstr32
    mov eax, [reg32]
    mov esi, hexstr
    mov ecx, 8               ; eight nibbles
 
hexloop:
    rol eax, 4               ; leftmost will
    mov ebx, eax             ; become rightmost
    and ebx, 0x0f ;
    mov bl, [esi + ebx]      ; index into hexstr
    mov [edi], bl
    inc edi
    dec ecx
    jnz hexloop
 
    mov esi, outstr32
    call sprint
    ret

[section .data]
xpos db 0
ypos db 15
hexstr db '0123456789ABCDEF'
outstr32 db '00000000', 0    ; register value
reg32 dd 0                   ; pass values to printreg32
%endif