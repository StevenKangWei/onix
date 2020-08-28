#ifndef ONIX_CONSOLE_H
#define ONIX_CONSOLE_H

#include <onix/type.h>

#define VGA_ADDRESS 0xB8000
#define VGA_MEMORY_SIZE 0x8000 /* 32K: B8000H -> BFFFFH */
#define VGA_BLOCK_SIZE 2
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_SIZE (VGA_WIDTH * VGA_HEIGHT)

#define SCROLL_UP 1
#define SCROLL_DOWN -1

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

#define CRTC_ADDR_REG 0x3D4 /* CRT Controller Registers - Addr Register */
#define CRTC_DATA_REG 0x3D5 /* CRT Controller Registers - Data Register */
#define START_ADDR_H 0xC    /* reg index of video mem start addr (MSB) */
#define START_ADDR_L 0xD    /* reg index of video mem start addr (LSB) */
#define CURSOR_H 0xE        /* reg index of cursor position (MSB) */
#define CURSOR_L 0xF        /* reg index of cursor position (LSB) */

typedef struct Console
{
    u32 start;
    u32 current;
    u32 limit;
    u32 cursor;
} _packed Console;

void init_console();
u32 get_cursor();

void out_char(Console *console, char ch);

extern Console kconsole;

#endif