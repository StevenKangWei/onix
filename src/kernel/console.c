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

u32 get_cursor()
{
    u32 pos = 0;
    // disable_interrupt();
    out_byte(CRTC_ADDR_REG, CURSOR_L);
    pos |= in_byte(CRTC_DATA_REG);
    out_byte(CRTC_ADDR_REG, CURSOR_H);
    pos |= ((u32)in_byte(CRTC_DATA_REG)) << 8;
    // enable_interrupt();
    return pos;
}

void out_char(Console *console, char ch)
{
    volatile char *video = (volatile char *)console->start + (console->cursor * VGA_BLOCK_SIZE);
    *video++ = ch;
    *video++ = COLOR_DEFAULT;
    // console->cursor++;
    // set_cursor(console->cursor);
}