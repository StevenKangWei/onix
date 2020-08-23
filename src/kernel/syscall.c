#include <onix/syscall.h>
#include <onix/stdio.h>

syscall syscall_table[NR_SYS_CALL] = {sys_get_ticks};

int sys_get_ticks()
{
    putchar('+');
    return 0;
}
