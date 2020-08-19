#ifndef ONIX_STDIO_H
#define ONIX_STDIO_H

#include <onix/type.h>

#define VGA_ADDRESS 0xB8000
#define VGA_LENGTH 0x8000 // 32kB
#define VGA_BLOCK_SIZE 2
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define COLOR_BLACK 0         // 0000b
#define COLOR_BLUE 1          // 0001b
#define COLOR_GREEN 2         // 0010b
#define COLOR_CYAN 3          // 0011b
#define COLOR_RED 4           // 0100b
#define COLOR_MAGENTA 5       // 0101b
#define COLOR_BROWN 6         // 0110b
#define COLOR_GRAY 7          // 0111b
#define COLOR_DARKGRAY 8      // 1000b
#define COLOR_LIGHT_BLUE 9    // 1001b
#define COLOR_LIBHT_GREEN 10  // 1010b
#define COLOR_LIBHT_CYAN 11   // 1011b
#define COLOR_LIBHT_RED 12    // 1100b
#define COLOR_LIBHT_PURPLE 13 // 1101b
#define COLOR_YELLOW 14       // 1110b
#define COLOR_WHITE 15        // 1111b

#define COLOR_DEFAULT (COLOR_BLACK << 4 | COLOR_GRAY)

u16 get_cursor();
u16 get_cursor_x();
u16 get_cursor_y();
void set_cursor(int x, int y);

void clear();

u16 get_color(int back, int front);

void put(char character, uchar color);

void putchar(char character);
int printf(const char *format, ...);

#endif