#ifndef ONIX_SYS_H
#define ONIX_SYS_H

#include <onix/type.h>
#include <onix/process.h>

#define NR_SYS_CALL 2

#define SENDING 0x02   /* set when proc trying to send */
#define RECEIVING 0x04 /* set when proc trying to recv */

typedef void *syscall;
extern void pause();
extern void halt();
extern void hang();

extern char read_byte(int addr);
extern void write_byte(int addr, char data);

// kernel system call
int sys_get_ticks();
int sys_write(char *buffer, int length, Process *process);

// kernel asm wrapper
void system_call();

// user system call
int get_ticks();
void write(char *buffer, int length);
#endif