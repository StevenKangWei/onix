#ifndef ONIX_IO_H
#define ONIX_IO_H

#include <onix/type.h>

extern void out_byte(u16 port, u8 value);
extern u8 in_byte(u16 port);

extern void disable_interrupt();
extern void enable_interrupt();

#endif