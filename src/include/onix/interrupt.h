#ifndef ONIX_INTERRUPT_H
#define ONIX_INTERRUPT_H

#include <onix/type.h>

#define IDT_BASE 0x00000000
#define IDT_SIZE 256

typedef struct IDTPointer
{
    u16 limite;
    u32 base;
} _packed IDTPointer;

typedef struct IDTDescriptor
{
    u16 offset0_15;
    u16 select;
    u16 type;
    u16 offset16_31;
} _packed IDTDescriptor;

void init_idt();

#endif