#include <onix/mode.h>
#include <onix/string.h>

u8 gdt_ptr[6];
GDTEntry gdt[GDT_SIZE];

void init_gdt()
{
    memcpy(&gdt, (void *)(*((u32 *)(&gdt_ptr[2]))), *((u16 *)(&gdt_ptr[0])) + 1);
    u16 *p_gdt_limit = (u16 *)(&gdt_ptr[0]);
    u32 *p_gdt_base = (u32 *)(&gdt_ptr[2]);
    *p_gdt_limit = GDT_SIZE * sizeof(GDTEntry) - 1;
    *p_gdt_base = (u32)&gdt;
}