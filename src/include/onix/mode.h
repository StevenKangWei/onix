#ifndef ONIX_MODE_H
#define ONIX_MODE_H

#include <onix/type.h>

#define GDT_SIZE 128
#define IDT_SIZE 256

/* 存储段描述符/系统段描述符 */
typedef struct GDTEntry /* 共 8 个字节 */
{
    u16 limit_low;       /* Limit */
    u16 base_low;        /* Base */
    u8 base_mid;         /* Base */
    u8 attr1;            /* P(1) DPL(2) DT(1) TYPE(4) */
    u8 limit_high_attr2; /* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
    u8 base_high;        /* Base */
} GDTEntry;

extern u8 gdt_ptr[6];
extern GDTEntry gdt[GDT_SIZE];

void init_gdt();
extern void load_gdt();
#endif