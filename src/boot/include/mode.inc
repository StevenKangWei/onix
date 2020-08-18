%ifndef ONIX_MODE_INC
%define ONIX_MODE_INC

%macro Descriptor 3 
    dw %2 & 0FFFFh
    dw %1 & 0FFFFh
    db (%1 >> 16) & 0FFh
    dw ((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)
    db (%1 >> 24) & 0FFh
%endmacro

%macro Gate 4 
    dw (%2 & 0FFFFh)
    dw %1
    dw (%3 & 1Fh) | ((%4 << 8) & 0FF00h)
    dw ((%2 >> 16) & 0FFFFh)
%endmacro

%endif