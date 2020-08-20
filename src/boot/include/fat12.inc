%ifndef ONIX_FAT12_INC
%define ONIX_FAT12_INC

%macro FAT12_HEADER 1
jmp %1
nop

;Remember, all numbers are in the little-endian format
BS_OEMName          DB 'Onix OS '   ; OEM name , 8 bytes
BPB_BytsPerSec      DW 512          ; The number of Bytes per sector.
BPB_SecPerClus      DB 1            ; Number of sectors per cluster
BPB_RsvdSecCnt      DW 1            ; Number of reserved sectors. 
                                    ;The boot record sectors are included in this value.
BPB_NumFATs         DB 2            ; Number of File Allocation Tables
BPB_RootEntCnt      DW 0xE0         ; Number of directory entries
BPB_TotSec16        DW 0xB40        ; The total sectors in the logical volume.
BPB_Media           DB 0xF0         ; media descriptor type
BPB_FATSz16         DW 9            ; Number of sectors per FAT. FAT12/FAT16 only.
BPB_SecPerTrk       DW 0x12           ; Number of sectors per track.
BPB_NumHeads        DW 2            ; Number of heads or sides on the storage media.
BPB_HiddSec         DD 0            ; Number of hidden sectors
BPB_TotSec32        DD 0            ; Large sector count. 
BS_DrvNum           DB 0            ; Drive number. BIOS interrupt 0x13,
BS_Reserved1        DB 0            ; Flags in Windows NT. Reserved otherwise.
BS_BootSig          DB 0x29          ; Signature
BS_VolID            DD 0            ; VolumeID 'Serial' number
BS_VolLab           DB 'Onix OS    '; Volume label string, must has 11 bytes
BS_FileSysType      DB 'FAT12   '   ; System identifier string, must has 8 bytes

FAT12_ROOT_SECTORS          equ 14
FAT12_SECTOR_INDEX_MBR      equ 0h
FAT12_SECTOR_INDEX_FAT1     equ 1
FAT12_SECTOR_INDEX_FAT2     equ 10
FAT12_SECTOR_INDEX_ROOT     equ 19
FAT12_SECTOR_INDEX_DELTA    equ 17
FAT12_SECTOR_INDEX_ENTRY    equ 33 ; 19 + 14

LOADER_SEGMENT  equ 0x9000
LOADER_OFFSET   equ 0x100
LOADER_ADDRESS  equ 0x90000

KERNEL_SEGMENT  equ 0x8000
KERNEL_OFFSET   equ 0x0
KERNEL_ADDRESS   equ 0x80400

%endmacro

%macro FAT12_READ_FILE 0
%include "bios.inc"
    BIOS_RESET_FLOPPY
    mov word[sector_index], FAT12_SECTOR_INDEX_ROOT
%%search_filename_in_root_directory:
    cmp word [root_dir_size], 0
    jz read_not_found
    dec word [root_dir_size]

    mov ax, MEMORY_SEGMENT
    mov es, ax
    mov bx, MEMORY_OFFSET
    mov ax, [sector_index]
    mov cl, 1

    call %%read_sector

    mov si, FILENAME
    mov di, MEMORY_OFFSET
    cld
    mov dx, 16 ; a sector contain 16 directory entry

%%search_compare_filename_in_entry:
    cmp dx, 0
    jz %%search_filename_next_sector
    dec dx

    mov cx, 11
%%search_compare_filename:
    cmp cx, 0
    jz %%read_filename_found

    dec cx
    lodsb
    cmp al, byte[es:di]
    jz %%search_compare_filename_continue
    jmp %%search_compare_filename_different

%%search_compare_filename_different:
    and di, 0xffe0
    add di, 20h ; size of entry
    mov si, FILENAME
    jmp %%search_compare_filename_in_entry

%%search_compare_filename_continue:
    inc di
    jmp %%search_compare_filename

%%search_filename_next_sector:
    add word [sector_index], 1
    jmp %%search_filename_in_root_directory

%%read_filename_found:
    mov si, MESSAGE_RETURN
    call bios_print
    mov si, MESSAGE_FOUND
    call bios_print

    mov ax, FAT12_ROOT_SECTORS
    and di, 0xffe0

    push eax
    mov eax, [es : di + 0x1c]; 0x1c offset of filesize
    mov dword [FILESIZE], eax
    pop eax

    add di, 0x1a
    mov cx, word [es:di]
    push cx
    add cx, ax
    add cx, FAT12_SECTOR_INDEX_DELTA

    mov ax, MEMORY_SEGMENT
    mov es, ax
    mov bx, MEMORY_OFFSET

    mov ax, cx

%%read_file:
    call %%print_dot
    mov cl, 1
    call %%read_sector

    pop ax 
    call %%get_next_cluster

    cmp ax, 0xfff
    jz read_success

    push ax
    mov dx, FAT12_ROOT_SECTORS
    add ax, dx
    add ax, FAT12_SECTOR_INDEX_DELTA
    add bx, [BPB_BytsPerSec]
    jnc %%read_file

%%next_segment
    push ax
    mov ax, es
    add ax, 0x1000
    mov es, ax
    pop ax
    jmp %%read_file

%%print_dot:
    push ax
    push bx
    mov ah, 0eh
    mov al, '.'
    mov bl, 0fh
    int 10h
    pop bx
    pop ax
    ret

%%get_next_cluster:
    push es
    push bx
    push ax

    mov ax, MEMORY_SEGMENT
    sub ax, 0x100
    mov es, ax
    pop ax

    mov byte [odd_even], 0
    mov bx, 3
    mul bx
    mov bx, 2
    div bx
    cmp dx, 0
    jz %%cluster_even
    mov byte [odd_even], 1
%%cluster_even:

    xor dx, dx
    mov bx, [BPB_BytsPerSec]
    div bx

    push dx
    mov bx, 0
    add ax, FAT12_SECTOR_INDEX_FAT1
    mov cl, 2
    call %%read_sector

    pop dx
    add bx, dx
    mov ax, [es:bx]
    cmp byte [odd_even], 1
    jnz %%cluster_odd
    shr ax, 4

%%cluster_odd:
    and ax, 0xfff
%%get_next_cluster_done:
    pop bx
    pop es
    ret

%%read_sector:
    push bp
    mov bp, sp
    sub esp, 2

    mov byte [bp - 2], cl; sector_size
    push bx
    mov bl, [BPB_SecPerTrk]
    div bl
    inc ah
    mov cl, ah
    mov dh, al
    shr al, 1
    mov ch, al
    and dh, 1
    pop bx

    mov dl, [BS_DrvNum]

    %%do_read_sector:

    mov ah, 2 ; read
    mov al, byte [bp - 2]; sector size 
    int 13h
    jc read_failure

    add esp, 2
    pop bp
    ret

sector_index dw 0;
odd_even     db 0;
root_dir_size dw FAT12_ROOT_SECTORS;

%endmacro

%endif