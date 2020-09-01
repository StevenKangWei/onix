#include <onix/syscall.h>
#include <onix/clock.h>
#include <onix/string.h>
#include <onix/assert.h>

syscall syscall_table[SYSCALL_SIZE] = {sys_pause, sys_get_ticks};

int sys_get_ticks()
{
    return kernel_ticks;
}