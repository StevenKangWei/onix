#include <onix/mode.h>
#include <onix/memory.h>

u32 seg2phys(u16 seg)
{
    Descriptor *desc = &gdt[seg >> 3];
    return (desc->base_high << 24 | desc->base_mid << 16 | desc->base_low);
}