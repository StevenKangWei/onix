#include <onix/io.h>
#include <onix/stdio.h>
#include <onix/audio.h>
#include <onix/type.h>
#include <onix/stdarg.h>
#include <onix/stdlib.h>

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
#ifdef ONIX_DEBUG
    return;
#endif
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
    u16 pos = get_cursor();
    u16 x = pos % VGA_WIDTH;
    u16 y = pos / VGA_WIDTH;

    switch (character)
    {
    case '\b':
        x = x >= 1 ? x - 1 : 0;
        set_cursor(x, y);
    case '\r':
        set_cursor(0, y);
    case '\n':
        set_cursor(0, y + 1);
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
        beep();
        break;
    default:
        put(character, COLOR_DEFAULT);
        break;
    }
}

void clear()
{
    set_cursor(0, 0);
    for (u32 i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        putchar(' ');
    }
    set_cursor(0, 0);
}

void print(const char *string)
{
    while (*string)
    {
        putchar(*string);
        string++;
    }
}

int printf(const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    va_list next = args;

    for (; *format != '\0'; format++)
    {
        if (*format != '%')
        {
            putchar(*format);
            continue;
        }

        format++;
        switch (*format)
        {
        case 'd':
        case 'i':
            itoa(va_arg(next, int), buffer, 10);
            print(buffer);
            break;
        case 'u':
            itoa(va_arg(next, uint), buffer, 10);
            print(buffer);
            break;
        case 'o':
            itoa(va_arg(next, uint), buffer, 8);
            print(buffer);
            break;
        case 'x':
        case 'X':
            itoa(va_arg(next, int), buffer, 16);
            print(buffer);
            break;
        case 'c':
            putchar(va_arg(next, char));
            break;
        case 's':
            print(va_arg(next, uchar *));
            break;
        case '%':
            putchar('%');
            break;
        default:
            break;
        }
    }
}