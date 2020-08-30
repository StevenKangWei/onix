#include <onix/console.h>
#include <onix/io.h>
#include <onix/stdio.h>

Console kconsole;

void init_console()
{
    kconsole.start = VGA_ADDRESS;
    kconsole.cursor = get_cursor();
    kconsole.current = kconsole.start;
    kconsole.limit = VGA_SIZE;
}

void set_start(u32 addr)
{
    out_byte(CRTC_ADDR_REG, START_ADDR_H);
    out_byte(CRTC_DATA_REG, (addr >> 8) & 0xFF);
    out_byte(CRTC_ADDR_REG, START_ADDR_L);
    out_byte(CRTC_DATA_REG, addr & 0xFF);
}

u32 get_cursor()
{
    u32 pos = 0;
    out_byte(CRTC_ADDR_REG, CURSOR_L);
    pos |= in_byte(CRTC_DATA_REG);
    out_byte(CRTC_ADDR_REG, CURSOR_H);
    pos |= ((u32)in_byte(CRTC_DATA_REG)) << 8;
    return pos;
}

void set_cursor(int pos)
{
    out_byte(CRTC_ADDR_REG, CURSOR_L);
    out_byte(CRTC_DATA_REG, (u8)(pos & 0xFF));
    out_byte(CRTC_ADDR_REG, CURSOR_H);
    out_byte(CRTC_DATA_REG, (u8)((pos >> 8) & 0xFF));
}

void set_cursor_coordinate(int x, int y)
{
    set_cursor(get_coordinate_pos(x, y));
}

u32 get_coordinate_pos(int x, int y)
{
    return y * VGA_WIDTH + x;
}

void scroll(Console *console, int direction)
{
    if (direction == SCROLL_DOWN)
    {
        if (console->current > console->start)
            console->current -= VGA_WIDTH;
        if (console->current < console->start)
            console->current = console->start;
    }
    else
    {
        bool flag = true;
        while (console->cursor >= console->limit)
        {
            flag = false;
            int length = VGA_WIDTH * VGA_BLOCK_SIZE;
            int rows = console->limit / VGA_WIDTH;
            int i;
            for (i = 1; i <= rows; i++)
            {
                volatile char *dest = (volatile char *)console->start + ((i - 1) * length);
                volatile char *src = (volatile char *)console->start + (i * length);
                memcpy(dest, src, length);
            }
            volatile char *dest = (volatile char *)console->start + ((i - 1) * length);
            for (; dest < i * length; dest++)
            {
                *(dest++) = ' ';
            }
            console->cursor -= VGA_WIDTH;
        }
        if (flag)
        {
            console->current += VGA_WIDTH;
        }
    }
    flush(console);
}

void flush(Console *console)
{
    set_cursor(console->cursor);
    set_start(console->current);
}

void clear(Console *console)
{
    console->cursor = 0;
    for (size_t i = 0; i < console->limit; i++)
    {
        out_char(console, ' ');
    }
    console->cursor = 0;
    console->current = console->start;
    flush(console);
}

void out_char(Console *console, char ch)
{
    volatile char *video = (volatile char *)console->start + (console->cursor * VGA_BLOCK_SIZE);
    *video++ = ch;
    *video++ = COLOR_DEFAULT;
    console->cursor++;
    set_cursor(console->cursor);
}

void set_char(char ch, uchar color, int x, int y)
{
    if (x < 0 || x >= VGA_WIDTH)
        return;
    if (y < 0 || y >= VGA_HEIGHT)
        return;
    u16 pos = y * VGA_WIDTH + x;
    volatile char *video = (volatile char *)VGA_ADDRESS + (pos * VGA_BLOCK_SIZE);
    *video++ = ch;
    *video++ = color;
}

void put_char(Console *console, char ch)
{
    u32 cursor = get_cursor();
    u16 x = cursor % VGA_WIDTH;
    u16 y = cursor / VGA_WIDTH;
    int i = 0;
    switch (ch)
    {
    case '\b':
        x = x >= 1 ? x - 1 : 0;
        console->cursor = get_coordinate_pos(x, y);
        set_char(' ', COLOR_DEFAULT, x, y);
        break;
    case '\r':
        i = x;
        while (i >= 0)
        {
            set_char(' ', COLOR_DEFAULT, i--, y);
        }
        console->cursor = get_coordinate_pos(0, y);
        break;
    case '\n':
        console->cursor = get_coordinate_pos(0, y + 1);
        break;
    case '\t':
        //table
        break;
    case '\v':
        // vertial table
        break;
    case '\f':
        console->cursor = get_coordinate_pos(0, y + 25);
        break;
    case '\a':
        // beep();
        break;
    default:
        out_char(console, ch);
        break;
    }

    int delta = console->current - console->start;
    if (console->cursor - delta >= VGA_SIZE)
    {
        scroll(console, SCROLL_UP);
    }

    flush(console);
}