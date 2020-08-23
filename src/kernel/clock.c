#include <onix/clock.h>
#include <onix/stdio.h>
#include <onix/process.h>
#include <onix/kernel.h>
#include <onix/time.h>

int kernel_ticks;

void init_clock()
{
    kernel_ticks = 0;
}

void clock_handler(int irq)
{
    kernel_ticks++;
    if (kernel_reenter > 1)
    {
        return;
    }
    process_ready++;
    if (process_ready >= process_table + PROCESS_SIZE)
    {
        process_ready = process_table;
    }
}