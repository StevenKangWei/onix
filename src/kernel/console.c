#include <onix/console.h>
#include <onix/io.h>

Console kconsole;

void init_console()
{
    kconsole.start = VGA_ADDRESS;
    kconsole.cursor = get_cursor();
    kconsole.current = kconsole.current;
    kconsole.limit = VGA_MEMORY_SIZE / VGA_BLOCK_SIZE;
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
    int pos = y * VGA_WIDTH + x;
    set_cursor(pos);
}

void flush(Console *console)
{
    set_cursor(console->cursor);
    set_start(console->start);
}

void out_char(Console *console, char ch)
{
    volatile char *video = (volatile char *)console->start + (console->cursor * VGA_BLOCK_SIZE);
    *video++ = ch;
    *video++ = COLOR_DEFAULT;
    console->cursor++;
    flush(console);
}

void put_char(Console *console, char ch)
{
    u16 x = SCAN_X(console->cursor);
    u16 y = SCAN_Y(console->cursor);

    switch (ch)
    {
    case '\b':
        x = x >= 1 ? x - 1 : 0;
        console->cursor = SCAN_POS(x, y);
        break;
    case '\r':
        console->cursor = SCAN_POS(0, y);
        break;
    case '\n':
        console->cursor = SCAN_POS(0, y + 1);
        break;
    case '\t':
        //table
        break;
    case '\v':
        // vertial table
        break;
    case '\f':
        // next page
        break;
    case '\a':
        // beep();
        break;
    default:
        out_char(console, ch);
        break;
    }
    flush(console);
}