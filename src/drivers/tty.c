#include <onix/tty.h>
#include <onix/keyboard.h>
#include <onix/io.h>
#include <onix/stdio.h>

TTY tty_table[NR_CONSOLES];
Console console_table[NR_CONSOLES];
Console *current_console;

void task_tty()
{
    TTY *tty;
    for (tty = tty_table; tty < tty_table + NR_CONSOLES; tty++)
    {
        init_tty(tty);
    }
    current_console = console_table;
    while (true)
    {
        for (tty = tty_table; tty < tty_table + NR_CONSOLES; tty++)
        {
            read_tty(tty);
            write_tty(tty);
        }
    }
}

void init_tty(TTY *tty)
{
    tty->count = 0;
    tty->head = tty->tail = tty->buffer;

    int index = tty - tty_table;
    tty->console = console_table + index;
}

void in_process(TTY *tty, u32 key)
{
    if (!(key & FLAG_EXT))
    {
        if (tty->count < TTY_IN_BYTES)
        {
            *(tty->head) = key;
            tty->head++;
            if (tty->head == tty->buffer + TTY_IN_BYTES)
            {
                tty->head = tty->buffer;
            }
            tty->count++;
        }
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

void out_char(Console *console, char ch)
{
    putchar(ch);
}

void read_tty(TTY *tty)
{
    if (tty->console == current_console)
    {
        read_keyboard(tty);
    }
}
void write_tty(TTY *tty)
{
    if (tty->count <= 0)
        return;
    char ch = *(tty->tail);
    tty->tail++;
    if (tty->tail == tty->buffer + TTY_IN_BYTES)
        tty->tail = tty->buffer;
    tty->count--;
    out_char(tty->console, ch);
}

void set_cursor(int pos)
{
    io_outb(CRTC_ADDR_REG, CURSOR_L);
    io_outb(CRTC_DATA_REG, (u8)(pos & 0xFF));
    io_outb(CRTC_ADDR_REG, CURSOR_H);
    io_outb(CRTC_DATA_REG, (u8)((pos >> 8) & 0xFF));
}

void set_cursor_coordinate(int x, int y)
{
    int pos = y * VGA_WIDTH + x;
    set_cursor(pos);
}