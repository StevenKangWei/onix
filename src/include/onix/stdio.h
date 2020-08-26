#ifndef ONIX_STDIO_H
#define ONIX_STDIO_H

#include <onix/type.h>
#include <onix/tty.h>

u16 get_cursor_x();
u16 get_cursor_y();

void clear();

u16 get_color(int back, int front);

void put(char character, uchar color);

void putchar(char character);
void setchar(char character, uchar color, int x, int y);
void print(const char *string);
void printf(const char *format, ...);

#endif