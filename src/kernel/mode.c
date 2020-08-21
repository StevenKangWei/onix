#include <onix/mode.h>
#include <onix/string.h>

DESCPointer gdt_ptr;
GDTDescriptor gdt[GDT_SIZE];

void init_gdt()
{
    memcpy(&gdt, (void *)gdt_ptr.base, gdt_ptr.limite + 1);
    gdt_ptr.limite = GDT_SIZE * sizeof(GDTDescriptor) - 1;
    gdt_ptr.base = (u32)&gdt;
}