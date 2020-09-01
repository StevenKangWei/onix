#ifndef ONIX_SYSCALL_H
#define ONIX_SYSCALL_H

#include <onix/process.h>

#define SYSCALL_SIZE 2

#define INT_VECTOR_SYS_CALL 0x90

typedef void *syscall;

typedef struct Message
{
    /* data */
} Message;

// message
void reset_message(Message *message);
void block_process(Process *process);
void unblock_process(Process *process);

// interactive interrupt
extern void _syscall();

// kernel mode
extern void sys_pause();
int sys_get_ticks();

// user mode
extern void pause();
extern int get_ticks();

#endif