#include <onix/io.h>
#include <onix/stdio.h>

u16 get_cursor()
{
    u16 pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((u16)inb(0x3D5)) << 8;
    return pos;
}

u16 get_cursor_x()
{
    u16 pos = get_cursor();
    return pos % VGA_WIDTH;
}
u16 get_cursor_y()
{
    u16 pos = get_cursor();
    return pos / VGA_WIDTH;
}

void set_cursor(int x, int y)
{
    u16 pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

u16 get_color(int back, int front)
{
    return back << 4 | front;
}

void put(char character, uchar color)
{
    u16 pos = get_cursor();
    volatile char *video = (volatile char *)VGA_ADDRESS + (pos * VGA_BLOCK_SIZE);
    *video++ = character;
    *video++ = color;

    // update cursor
    pos++;
    u16 x = pos % VGA_WIDTH;
    u16 y = pos / VGA_WIDTH;
    set_cursor(x, y);
}

void putchar(char character)
{
    put(character, COLOR_DEFAULT);
}

void clear()
{
    for (size_t i = VGA_ADDRESS; i < VGA_ADDRESS + VGA_LENGTH; i += VGA_BLOCK_SIZE)
    {
        putchar(' ');
    }
}

int printf(const char *format, ...)
{
    int i = 0;
    while (format[i])
    {
        putchar(format[i++]);
    }
}