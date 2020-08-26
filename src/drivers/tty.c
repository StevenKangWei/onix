#include <onix/tty.h>
#include <onix/keyboard.h>
#include <onix/io.h>
#include <onix/stdio.h>

TTY tty_table[NR_CONSOLES];
Console console_table[NR_CONSOLES];
Console *current_console;

void init_screen(TTY *tty)
{
    int index = tty - tty_table;
    printf("init screen index %d\n\0", index);

    tty->console = console_table + index;

    int vga_size = VGA_SIZE >> 1;

    int memory_size = vga_size / NR_CONSOLES;
    tty->console->start = index * memory_size;
    tty->console->limit = memory_size;
    tty->console->current = tty->console->start;
    tty->console->cursor = tty->console->start;

    if (index == 0)
    {
        tty->console->cursor = 0;
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
    set_cursor(current_console->cursor);
    set_start(current_console->start);
}

void init_tty(TTY *tty)
{
    tty->count = 0;
    tty->head = tty->tail = tty->buffer;

    init_screen(tty);
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
                // io_cli();
                io_outb(CRTC_ADDR_REG, START_ADDR_H);
                io_outb(CRTC_DATA_REG, ((80 * 15) >> 8) & 0xFF);
                io_outb(CRTC_ADDR_REG, START_ADDR_L);
                io_outb(CRTC_DATA_REG, (80 * 15) & 0xFF);
                // io_sti();
            }
            break;
        case DOWN:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                /* Shift+Down, do nothing */
            }
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

void out_char(Console *console, char ch)
{
    volatile char *video = (volatile char *)VGA_ADDRESS + (console->cursor * VGA_BLOCK_SIZE);
    *video++ = ch;
    *video++ = COLOR_DEFAULT;
    console->cursor++;
    set_cursor(console->cursor);
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