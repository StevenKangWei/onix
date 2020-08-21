#ifndef ONIX_IO_H
#define ONIX_IO_H

#include <onix/type.h>

void io_outb(u16 port, u8 value);
u8 io_inb(u16 port);
int write(char *buf, unsigned int len);

#endif