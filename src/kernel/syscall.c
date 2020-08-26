#include <onix/syscall.h>
#include <onix/stdio.h>
#include <onix/clock.h>

syscall syscall_table[NR_SYS_CALL] = {sys_get_ticks};

int sys_get_ticks()
{
    return kernel_ticks;
}

void hang()
{
    while (true)
    {

    }
}