#include <onix/syscall.h>
#include <onix/clock.h>
#include <onix/string.h>
#include <onix/assert.h>

syscall syscall_table[SYSCALL_SIZE] = {sys_pause, sys_get_ticks};

void reset_message(Message *message)
{
    memset(message, 0, sizeof(Message));
}

void block_process(Process *process)
{
    assert(process->flags);
    schedule();
}

void unblock_process(Process *process)
{
    assert(process->flags == 0);
}

int sys_get_ticks()
{
    return kernel_ticks;
}