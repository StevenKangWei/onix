#include <onix/memory.h>
#include <onix/mode.h>
#include <onix/assert.h>

u32 seg2phys(u16 seg)
{
    Descriptor *desc = &gdt[seg >> 3];
    return (desc->base_high << 24 | desc->base_mid << 16 | desc->base_low);
}

u32 ldt_seg_linear(Process *process, int index)
{
    Descriptor *d = &process->ldt[index];
    return d->base_high << 24 | d->base_mid << 16 | d->base_low;
}

void *va2la(Process *process, void *va) // virtual address
{
    u32 seg_base = ldt_seg_linear(process, INDEX_LDT_RW);
    u32 la = seg_base + (u32)va;
    if (process->pid < PROCESS_SIZE)
    {
        assert(la == (u32)va);
    }
    return (void *)la;
}