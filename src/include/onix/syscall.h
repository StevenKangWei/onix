#ifndef ONIX_SYSCALL_H
#define ONIX_SYSCALL_H

#include <onix/process.h>

#define SYSCALL_SIZE 3

#define INT_VECTOR_SYS_CALL 0x90

typedef void *syscall;

// interactive interrupt
extern void _syscall();

// kernel mode
extern void _sys_pause();
int sys_sendrecv(int type, int peer, Message *message, Process *process);

// user mode
extern void sys_pause();
extern int sys_get_ticks();
extern int sys_test_syscall();
extern int get_message_ticks();

// system task
void task_syscall();

#endif