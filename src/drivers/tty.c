#include <onix/tty.h>
#include <onix/keyboard.h>
#include <onix/io.h>
#include <onix/stdio.h>
#include <onix/string.h>

TTY tty_table[NR_CONSOLES];
Console console_table[NR_CONSOLES];
Console *current_console;

void init_screen(TTY *tty)
{
    int index = tty - tty_table;
    tty->console = console_table + index;

    int memory_size = VGA_ROWS * VGA_WIDTH;

    tty->console->start = index * memory_size;
    tty->console->limit = memory_size;
    tty->console->current = tty->console->start;
    tty->console->cursor = tty->console->start;

    if (index == 0)
    {
        tty->console->cursor = get_cursor();
    }
    else
    {
        out_char(tty->console, index + '0');
        out_char(tty->console, '#');
    }
}

void task_tty()
{
    TTY *tty;
    for (tty = tty_table; tty < tty_table + NR_CONSOLES; tty++)
    {
        init_tty(tty);
    }
    select_console(0);
    while (true)
    {
        for (tty = tty_table; tty < tty_table + NR_CONSOLES; tty++)
        {
            read_tty(tty);
            write_tty(tty);
        }
    }
}

void select_console(int index)
{
    if (index < 0 || index >= NR_CONSOLES)
        return;
    current_console = console_table + index;
    flush(current_console);
}

void init_tty(TTY *tty)
{
    tty->count = 0;
    tty->head = tty->tail = tty->buffer;

    init_screen(tty);
}

void put_key(TTY *tty, u32 key)
{
    if (tty->count >= TTY_IN_BYTES)
        return;
    *(tty->head) = key;
    tty->head++;
    if (tty->head == tty->buffer + TTY_IN_BYTES)
        tty->head = tty->buffer;
    tty->count++;
}

void in_process(TTY *tty, u32 key)
{
    if (!(key & FLAG_EXT))
    {
        if (tty->count < TTY_IN_BYTES)
        {
            *(tty->head) = key;
            tty->head++;
            if (tty->head == tty->buffer + TTY_IN_BYTES)
            {
                tty->head = tty->buffer;
            }
            tty->count++;
        }
    }
    else
    {
        int raw_code = key & MASK_RAW;
        switch (raw_code)
        {
        case UP:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                scroll_screen(tty->console, SCROLL_DOWN);
            }
            break;
        case DOWN:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                scroll_screen(tty->console, SCROLL_UP);
            }
            break;
        case LEFT:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                int index = (current_console - console_table) - 1;
                index %= NR_CONSOLES;
                select_console(index);
            }
            break;
        case RIGHT:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                int index = (current_console - console_table) + 1;
                index %= NR_CONSOLES;
                select_console(index);
            }
        case ENTER:
            put_key(tty, '\n');
            break;
        case BACKSPACE:
            put_key(tty, '\b');
            break;
        case F1:
        case F2:
        case F3:
        case F4:
        case F5:
        case F6:
        case F7:
        case F8:
        case F9:
        case F10:
        case F11:
        case F12:
            /* Alt + F1~F12 */
            if ((key & FLAG_ALT_L) || (key & FLAG_ALT_R))
            {
                select_console(raw_code - F1);
            }
            break;
        default:
            break;
        }
    }
}

void put_char(Console *console, char ch)
{
    volatile char *video = (volatile char *)VGA_ADDRESS + (console->cursor * VGA_BLOCK_SIZE);
    *video++ = ch;
    *video++ = COLOR_DEFAULT;
    console->cursor++;
}

void out_char(Console *console, char ch)
{
    u16 pos = console->cursor;
    u16 x = pos % VGA_WIDTH;
    u16 y = pos / VGA_WIDTH;

    switch (ch)
    {
    case '\b':
        x = x >= 1 ? x - 1 : 0;
        console->cursor = (y * VGA_WIDTH) + x;
        putchar(' ');
        console->cursor = (y * VGA_WIDTH) + x;
        break;
    case '\r':
        console->cursor = (y * VGA_WIDTH);
        break;
    case '\n':
        console->cursor = (y + 1) * VGA_WIDTH;
        while ((console->cursor - console->start) >= console->limit)
        {
            scroll(console, 1);
            console->cursor -= VGA_WIDTH;
        }
        break;
    case '\t':
        //table
        break;
    case '\v':
        // vertial table
        break;
    case '\f':
        console->cursor = (y + 25) * VGA_WIDTH;
        while ((console->cursor - console->start) >= console->limit)
        {
            scroll(console, 1);
            console->cursor -= VGA_WIDTH;
        }
        break;
    case '\a':
        beep();
        break;
    default:
        put_char(console, ch);
        break;
    }

    while (console->cursor >= console->current + VGA_SIZE)
    {
        scroll_screen(console, SCROLL_DOWN);
    }
    flush(console);
}

void flush(Console *console)
{
    if (console == current_console)
    {
        set_cursor(console->cursor);
        set_start(console->start);
    }
}

void scroll(Console *console, int row)
{
    if (row > VGA_HEIGHT)
        return;
    u32 start = 0;
    if (console != NULL)
    {
        start = console->start;
    }

    int length = VGA_WIDTH * VGA_BLOCK_SIZE;
    int rows = VGA_HEIGHT;
    if (console != NULL)
        rows = console->limit / VGA_WIDTH - 1;
    int i = 0;
    for (i = 0; i < rows; i++)
    {
        volatile char *dest = (volatile char *)VGA_ADDRESS + start + (i * length);
        volatile char *src = (volatile char *)VGA_ADDRESS + start + ((i + row) * length);
        memcpy(dest, src, length);
    }
    volatile char *dest = (volatile char *)VGA_ADDRESS + start + (i * length);
    memset(dest, 0, length);
}

void read_tty(TTY *tty)
{
    if (tty->console == current_console)
    {
        read_keyboard(tty);
    }
}
void write_tty(TTY *tty)
{
    if (tty->count <= 0)
        return;
    char ch = *(tty->tail);
    tty->tail++;
    if (tty->tail == tty->buffer + TTY_IN_BYTES)
        tty->tail = tty->buffer;
    tty->count--;
    out_char(tty->console, ch);
}

void set_cursor(int pos)
{
    // io_cli();
    io_outb(CRTC_ADDR_REG, CURSOR_L);
    io_outb(CRTC_DATA_REG, (u8)(pos & 0xFF));
    io_outb(CRTC_ADDR_REG, CURSOR_H);
    io_outb(CRTC_DATA_REG, (u8)((pos >> 8) & 0xFF));
    // io_sti();
}

u32 get_cursor()
{
    u32 pos = 0;
    io_outb(CRTC_ADDR_REG, CURSOR_L);
    pos |= io_inb(CRTC_DATA_REG);
    io_outb(CRTC_ADDR_REG, CURSOR_H);
    pos |= ((u32)io_inb(CRTC_DATA_REG)) << 8;
    return pos;
}

void set_cursor_coordinate(int x, int y)
{
    int pos = y * VGA_WIDTH + x;
    set_cursor(pos);
}

void set_start(int addr)
{
    io_outb(CRTC_ADDR_REG, START_ADDR_H);
    io_outb(CRTC_DATA_REG, (addr >> 8) & 0xFF);
    io_outb(CRTC_ADDR_REG, START_ADDR_L);
    io_outb(CRTC_DATA_REG, addr & 0xFF);
}

void scroll_screen(Console *console, int direction)
{
    if (direction == SCROLL_UP)
    {
        if (console->current > console->start)
            console->current -= VGA_WIDTH;
    }
    else
    {
        if (console->current + VGA_SIZE < console->start + console->limit)
        {
            console->current += VGA_WIDTH;
        }
    }
    flush(console);
}

void tty_write(TTY *tty, char *buffer, int length)
{
    char *p = buffer;
    int i = length;

    while (i)
    {
        out_char(tty->console, *p++);
        i--;
    }
}