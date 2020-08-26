#ifndef ONIX_TTY_H
#define ONIX_TTY_H
#include <onix/type.h>

#define VGA_ADDRESS 0xB8000
#define VGA_LENGTH 2000 //
#define VGA_SIZE 0x8000 /* 32K: B8000H -> BFFFFH */
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

#define CRTC_ADDR_REG 0x3D4 /* CRT Controller Registers - Addr Register */
#define CRTC_DATA_REG 0x3D5 /* CRT Controller Registers - Data Register */
#define START_ADDR_H 0xC    /* reg index of video mem start addr (MSB) */
#define START_ADDR_L 0xD    /* reg index of video mem start addr (LSB) */
#define CURSOR_H 0xE        /* reg index of cursor position (MSB) */
#define CURSOR_L 0xF        /* reg index of cursor position (LSB) */

#define NR_CONSOLES 3 /* consoles */

#define TTY_IN_BYTES 256 /* tty input queue size */

typedef struct Console
{
    unsigned int start;
    unsigned int current;
    unsigned int limit;
    unsigned int cursor;
} _packed Console;

/* TTY */
typedef struct TTY
{
    u32 buffer[TTY_IN_BYTES];
    u32 *head;
    u32 *tail;
    int count;

    Console *console;
} _packed TTY;

extern TTY tty_table[];
extern Console console_table[];
extern Console *current_console;

void out_char(Console *console, char ch);

void task_tty();
bool is_current_console(Console *console);

void init_tty(TTY *tty);
void read_tty(TTY *tty);
void write_tty(TTY *tty);
void in_process(TTY *tty, u32 key);

void select_console(int index);
void init_screen(TTY *tty);

void set_start(int addr);
u32 get_cursor();
void set_cursor(int pos);
void set_cursor_coordinate(int x, int y);

#endif