#ifndef ONIX_MODE_H
#define ONIX_MODE_H

#include <onix/type.h>

#define GDT_SIZE 128
#define LDT_SIZE 2

#define INDEX_LDT_C 0
#define INDEX_LDT_RW 1

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

#define SA_RPL_MASK 0xFFFC
#define SA_RPL0 0
#define SA_RPL1 1
#define SA_RPL2 2
#define SA_RPL3 3

#define SA_TI_MASK 0xFFFB
#define SA_TIG 0
#define SA_TIL 4

#define PG_P 1
#define PG_RWR 0
#define PG_RWW 2
#define PG_USS 0
#define PG_USU 4

#define GDT_INDEX_DUMMY 0
#define GDT_INDEX_CODE 1
#define GDT_INDEX_DATA 2
#define GDT_INDEX_VIDEO 3
#define GDT_INDEX_TSS 4
#define GDT_INDEX_LDT 5

#define SELECTOR_DUMMY 0
#define SELECTOR_CODE 0x08
#define SELECTOR_DATA 0x10
#define SELECTOR_VIDEO (0x18 + 3)
#define SELECTOR_TSS 0x20
#define SELECTOR_LDT 0x28

#define PRIVILEGE_KERNEL 0
#define PRIVILEGE_TASK 1
#define PRIVILEGE_USER 3

#define RPL_KERNEL SA_RPL0
#define RPL_TASK SA_RPL1
#define RPL_USER SA_RPL3

typedef struct Descriptor
{
    u16 limit_low;       /* Limit */
    u16 base_low;        /* Base */
    u8 base_mid;         /* Base */
    u8 attr1;            /* P(1) DPL(2) DT(1) TYPE(4) */
    u8 limit_high_attr2; /* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
    u8 base_high;        /* Base */
} _packed Descriptor;

typedef struct Pointer
{
    u16 limit;
    u32 base;
} _packed Pointer;

typedef struct Gate
{
    u16 offset_low; /* Offset Low */
    u16 selector;   /* Selector */
    u8 dcount;
    u8 attr;         /* P(1) DPL(2) DT(1) TYPE(4) */
    u16 offset_high; /* Offset High */
} _packed Gate;

extern Pointer gdt_ptr;
extern Descriptor gdt[GDT_SIZE];

void init_gdt();
void init_descriptor(Descriptor *desc, u32 base, u32 limit, u16 attribute);

extern void load_gdt(Pointer *gdt_ptr);
extern void load_idt(Pointer *idt_ptr);
extern void save_gdt(Pointer *gdt_ptr);
extern void load_tss(uint selector);

#endif