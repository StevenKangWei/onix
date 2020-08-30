#ifndef ONIX_SYSCALL_H
#define ONIX_SYSCALL_H

#define SYSCALL_SIZE 1

#define INT_VECTOR_SYS_CALL 0x90

typedef void *syscall;

extern void pause();

extern void _syscall();
int sys_get_ticks();

extern int get_ticks();

#endif