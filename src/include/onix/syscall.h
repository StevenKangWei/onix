#ifndef ONIX_SYS_H
#define ONIX_SYS_H

#include <onix/type.h>

#define NR_SYS_CALL 1
typedef void *syscall;
extern void pause();
extern void halt();
extern void hang();

extern char read_byte(int addr);
extern void write_byte(int addr, char data);

int sys_get_ticks();

void sys_call();
int get_ticks();

#endif