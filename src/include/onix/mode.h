#ifndef ONIX_MODE_H
#define ONIX_MODE_H

#include <onix/type.h>

#define GDT_SIZE 128
#define IDT_SIZE 256

#define DA_DPL0 0x00
#define DA_DPL1 0x20
#define DA_DPL2 0x40
#define DA_DPL3 0x60

#define DA_DR 0x90
#define DA_DRW 0x92
#define DA_DRWA 0x93
#define DA_C 0x98
#define DA_CR 0x9A
#define DA_CCO 0x9C
#define DA_CCOR 0x9E

#define DA_LDT 0x82
#define DA_TaskGate 0x85
#define DA_386TSS 0x89
#define DA_386CGate 0x8C
#define DA_386IGate 0x8E
#define DA_386TGate 0x8F

#define SA_RPL0 0
#define SA_RPL1 1
#define SA_RPL2 2
#define SA_RPL3 3

#define SA_TIG 0
#define SA_TIL 4

#define PG_P 1
#define PG_RWR 0
#define PG_RWW 2
#define PG_USS 0
#define PG_USU 4

typedef struct GDTDescriptor
{
    u16 limit_low;       /* Limit */
    u16 base_low;        /* Base */
    u8 base_mid;         /* Base */
    u8 attr1;            /* P(1) DPL(2) DT(1) TYPE(4) */
    u8 limit_high_attr2; /* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
    u8 base_high;        /* Base */
} _packed GDTDescriptor;

typedef struct GDTPointer
{
    u16 limite;
    u32 base;
} _packed GDTPointer;

extern GDTPointer gdt_ptr;
extern GDTDescriptor gdt[GDT_SIZE];

void init_gdt();
extern void load_gdt();

#endif