#include <onix/clock.h>
#include <onix/process.h>
#include <onix/interrupt.h>
#include <onix/stdio.h>

int kernel_ticks;

void init_clock()
{
    kernel_ticks = 0;
    put_irq_handler(CLOCK_IRQ, clock_handler);
    enable_irq(CLOCK_IRQ);
}

void clock_handler(int irq)
{
    kernel_ticks++;
    if (interrupt_enter > 1)
    {
        return;
    }
    return;
    process_ready++;
    if (process_ready >= process_table + PROCESS_SIZE)
    {
        process_ready = process_table;
    }
}