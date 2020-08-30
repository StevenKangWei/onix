#include <onix/syscall.h>
#include <onix/clock.h>

syscall syscall_table[SYSCALL_SIZE] = {sys_get_ticks};

int sys_get_ticks()
{
    return kernel_ticks;
}