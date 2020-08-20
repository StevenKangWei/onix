#ifndef ONIX_IO_H
#define ONIX_IO_H

#include <onix/type.h>

void outb(u16 port, u8 value);
u8 inb(u16 port);
int write(char *buf, unsigned int len);

#endif