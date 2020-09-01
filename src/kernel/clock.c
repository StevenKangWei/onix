#include <onix/clock.h>
#include <onix/process.h>
#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/stdio.h>

int kernel_ticks;

void init_clock()
{
    kernel_ticks = 0;

    out_byte(TIMER_MODE, RATE_GENERATOR);
    out_byte(TIMER0, (u8)(TIMER_FREQ / HZ));
    out_byte(TIMER0, (u8)((TIMER_FREQ / HZ) >> 8));

    put_irq_handler(CLOCK_IRQ, clock_handler);
    enable_irq(CLOCK_IRQ);
}

void clock_handler(int irq)
{
    kernel_ticks++;
    process_ready->ticks--;

    if (interrupt_count > 1)
    {
        return;
    }
    if (process_ready->ticks > 0)
        return;
    schedule();
}