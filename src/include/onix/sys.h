#ifndef ONIX_SYS_H
#define ONIX_SYS_H

#include <onix/type.h>

extern void pause();
extern void halt();
extern void hang();

extern char read_byte(int addr);
extern void write_byte(int addr, char data);

#endif