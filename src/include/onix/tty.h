#ifndef ONIX_TTY_H
#define ONIX_TTY_H
#include <onix/type.h>

#define CRTC_ADDR_REG 0x3D4 /* CRT Controller Registers - Addr Register */
#define CRTC_DATA_REG 0x3D5 /* CRT Controller Registers - Data Register */
#define START_ADDR_H 0xC    /* reg index of video mem start addr (MSB) */
#define START_ADDR_L 0xD    /* reg index of video mem start addr (LSB) */
#define CURSOR_H 0xE        /* reg index of cursor position (MSB) */
#define CURSOR_L 0xF        /* reg index of cursor position (LSB) */
#define V_MEM_BASE 0xB8000  /* base of color video memory */
#define V_MEM_SIZE 0x8000   /* 32K: B8000H -> BFFFFH */

#define NR_CONSOLES 3 /* consoles */

#define TTY_IN_BYTES 256 /* tty input queue size */

typedef struct Console
{
    unsigned int current;
    unsigned int addr;
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

void out_char(Console *console, char ch);

void task_tty();
bool is_current_console(Console *console);

void init_tty(TTY *tty);
void read_tty(TTY *tty);
void write_tty(TTY *tty);
void in_process(TTY *tty, u32 key);

void set_cursor(int pos);
void set_cursor_coordinate(int x, int y);

#endif