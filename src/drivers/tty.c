#include <onix/tty.h>
#include <onix/keyboard.h>
#include <onix/io.h>

void task_tty()
{
    while (true)
    {
        read_keyboard();
    }
}

void in_process(u32 key)
{
    if (!(key & FLAG_EXT))
    {
       
    }

    if (!(key & FLAG_EXT))
    {
        putchar(key & 0xFF);
    }
    else
    {
        int raw_code = key & MASK_RAW;
        switch (raw_code)
        {
        case UP:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                io_cli();
                io_outb(CRTC_ADDR_REG, START_ADDR_H);
                io_outb(CRTC_DATA_REG, ((80 * 15) >> 8) & 0xFF);
                io_outb(CRTC_ADDR_REG, START_ADDR_L);
                io_outb(CRTC_DATA_REG, (80 * 15) & 0xFF);
                io_sti();
            }
            break;
        case DOWN:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                /* Shift+Down, do nothing */
            }
            break;
        default:
            break;
        }
    }
}