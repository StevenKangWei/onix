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

%endmacro

%endif