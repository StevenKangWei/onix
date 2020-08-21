#include <onix/mode.h>
#include <onix/string.h>
#include <onix/process.h>
#include <onix/memory.h>
#include <onix/stdio.h>

DPointer gdt_ptr;
Descriptor gdt[GDT_SIZE];
TSS tss;

void init_gdt()
{
    memcpy(&gdt, (void *)gdt_ptr.base, gdt_ptr.limite + 1);
    gdt_ptr.limite = GDT_SIZE * sizeof(Descriptor) - 1;
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

void init_tss()
{   

    printf("Initilaizing TSS...\n\0");
    memset(&tss, 0, sizeof(tss));
    tss.ss0 = SELECTOR_DATA;

    printf("TTS -> %x:%x\n\0", vir2phys(seg2phys(SELECTOR_DATA), &tss), sizeof(tss) - 1);

    init_descriptor(
        &gdt[GDT_INDEX_TSS],
        vir2phys(seg2phys(SELECTOR_DATA), &tss),
        sizeof(tss) - 1,
        DA_386TSS);
    tss.iobase = sizeof(tss);
}

void init_ldt()
{
    printf("Initilaizing LDT...\n\0");
    init_descriptor(
        &gdt[GDT_INDEX_LDT],
        vir2phys(seg2phys(SELECTOR_DATA), processes[0].ldt),
        LDT_SIZE * sizeof(Descriptor) - 1,
        DA_LDT);
}