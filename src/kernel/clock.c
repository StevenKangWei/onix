#include <onix/clock.h>
#include <onix/stdio.h>
#include <onix/process.h>
#include <onix/kernel.h>
#include <onix/time.h>
#include <onix/io.h>

int kernel_ticks;

void init_clock()
{
    kernel_ticks = 0;

    io_outb(TIMER_MODE, RATE_GENERATOR);
    io_outb(TIMER0, (u8)(TIMER_FREQ / HZ));
    io_outb(TIMER0, (u8)((TIMER_FREQ / HZ) >> 8));
}

void clock_handler(int irq)
{
    kernel_ticks++;
    process_ready->ticks--;
    if (kernel_reenter > 1)
    {
        return;
    }
    schedule();
}