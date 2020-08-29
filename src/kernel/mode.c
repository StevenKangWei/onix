#include <onix/mode.h>
#include <onix/string.h>
#include <onix/stdio.h>

Pointer gdt_ptr;
Descriptor gdt[GDT_SIZE];
TSS tss;

void init_gdt()
{
    kprintf("Initializing GDT...\n\0");
    memcpy(&gdt, (void *)gdt_ptr.base, gdt_ptr.limit + 1);
    gdt_ptr.limit = GDT_SIZE * sizeof(Descriptor) - 1;
    gdt_ptr.base = (u32)&gdt;
}

void init_descriptor(Descriptor *desc, u32 base, u32 limit, u16 attribute)
{
    desc->limit_low = limit & 0x0FFFF;
    desc->base_low = base & 0x0FFFF;
    desc->base_mid = (base >> 16) & 0x0FF;
    desc->attr1 = attribute & 0xFF;
    desc->limit_high_attr2 = ((limit >> 16) & 0x0F) | ((attribute >> 8) & 0xF0);
    desc->base_high = (base >> 24) & 0x0FF;
}
