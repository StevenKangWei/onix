void __kernel_entry()
{
    _print_kernel();
}

#include <onix/io.h>

const static u16 VGA_WIDTH = 80;

void update_cursor(int x, int y)
{
    u16 pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

void _print_kernel()
{
    char *string = "Hello, Onix!!!\0";
    int color = 0x04; //black + red
    volatile char *video = (volatile char *)0xB8000;
    while (*string != 0)
    {
        *video++ = *string++;
        *video++ = color;
    }
    update_cursor(14, 0);
}