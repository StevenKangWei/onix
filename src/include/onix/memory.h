#ifndef ONIX_MEMORY_H
#define ONIX_MEMORY_H

#include <onix/type.h>

#define vir2phys(seg_base, vir) (u32)(((u32)seg_base) + (u32)(vir))

u32 seg2phys(u16 seg);

#endif