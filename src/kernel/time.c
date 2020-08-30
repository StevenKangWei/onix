#include <onix/time.h>
#include <onix/clock.h>
#include <onix/syscall.h>
#include <onix/stdio.h>
#include <onix/process.h>

void sleep(int milliseconds)
{
    int start = get_ticks();
    while (true)
    {
        int current = get_ticks();
        int interval = current - start;
        if (interval * 1000 / HZ > milliseconds)
            break;
        pause();
        schedule();
    }
}

void delay(int milliseconds)
{
    int start = get_ticks();
    while (true)
    {
        int current = get_ticks();
        int interval = current - start;
        if (interval * 1000 / HZ > milliseconds)
            break;
        schedule();
    }
}