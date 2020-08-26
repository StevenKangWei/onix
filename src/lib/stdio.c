#include <onix/io.h>
#include <onix/stdio.h>
#include <onix/audio.h>
#include <onix/type.h>
#include <onix/stdarg.h>
#include <onix/stdlib.h>
#include <onix/string.h>
#include <onix/tty.h>

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

u16 get_color(int back, int front)
{
    return back << 4 | front;
}

void scroll(int row)
{
    if (row > VGA_HEIGHT)
        return;

    int length = VGA_WIDTH * VGA_BLOCK_SIZE;
    for (size_t i = 0; i < VGA_HEIGHT; i++)
    {
        volatile char *dest = (volatile char *)VGA_ADDRESS + (i * length);
        volatile char *src = (volatile char *)VGA_ADDRESS + ((i + row) * length);
        memcpy(dest, src, length);
    }
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
    if (y >= VGA_HEIGHT)
    {
        scroll(1);
        y--;
    }
    set_cursor(pos);
}

void setchar(char character, uchar color, int x, int y)
{
    if (x < 0 || x >= VGA_WIDTH)
        return;
    if (y < 0 || y >= VGA_HEIGHT)
        return;
    u16 pos = y * VGA_WIDTH + x;
    volatile char *video = (volatile char *)VGA_ADDRESS + (pos * VGA_BLOCK_SIZE);
    *video++ = character;
    *video++ = color;
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
        set_cursor_coordinate(x, y);
        break;
    case '\r':
        set_cursor_coordinate(0, y);
        break;
    case '\n':
        set_cursor_coordinate(0, y + 1);
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
        out_char(current_console, character);
        break;
    }
}

void clear()
{
    set_cursor(0);
    for (u32 i = 0; i < VGA_LENGTH; i++)
    {
        putchar(' ');
    }
    set_cursor(0);
}

void print(const char *string)
{
    while (*string)
    {
        putchar(*string);
        string++;
    }
}

void printf(const char *format, ...)
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
            print(va_arg(next, char *));
            break;
        case '%':
            putchar('%');
            break;
        default:
            break;
        }
    }
}