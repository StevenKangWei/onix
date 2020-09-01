#include <onix/time.h>
#include <onix/clock.h>
#include <onix/syscall.h>
#include <onix/stdio.h>
#include <onix/process.h>

void sleep(int milliseconds)
{
    int start = sys_get_ticks();
    while (true)
    {
        int current = sys_get_ticks();
        int interval = current - start;
        if (interval * 1000 / HZ > milliseconds)
            break;
        sys_pause();
    }
}

void delay(int milliseconds)
{
    int start = sys_get_ticks();
    while (true)
    {
        int current = sys_get_ticks();
        int interval = current - start;
        if (interval * 1000 / HZ > milliseconds)
            break;
    }
}