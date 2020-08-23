#include <onix/clock.h>
#include <onix/stdio.h>
#include <onix/process.h>
#include <onix/kernel.h>
#include <onix/time.h>

void print_singal(int i)
{
    volatile char *video = (volatile char *)VGA_ADDRESS + (((80 * i) - 1) * VGA_BLOCK_SIZE);
    *(video + 1) = COLOR_DEFAULT;
    if (*video > 0x39 || *video < 0x30)
    {
        *video = 0x2f;
    }
    *video = (*video + 1);
}

void clock_handler(int irq)
{
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