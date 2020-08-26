#include <onix/syscall.h>
#include <onix/stdio.h>
#include <onix/clock.h>
#include <onix/process.h>
#include <onix/tty.h>

syscall syscall_table[NR_SYS_CALL] = {sys_get_ticks, sys_write};

void hang()
{
    while (true)
    {
    }
}

int sys_get_ticks()
{
    return kernel_ticks;
}

int sys_write(char *buffer, int length, Process *process)
{
    tty_write(&tty_table[process->tty_index], buffer, buffer);
    return 0;
}