#include <onix/tty.h>
#include <onix/type.h>
#include <onix/keyboard.h>
#include <onix/keymap.h>
#include <onix/console.h>
#include <onix/stdio.h>
#include <onix/string.h>

char command[TTY_COMMAND_SIZE];

void task_tty()
{
    init_tty();
    while (true)
    {
        read_keyboard(key_handler);
    }
}

void init_tty()
{
    memset(command, 0, TTY_COMMAND_SIZE);
}

void key_handler(int key)
{
    if (!(key & FLAG_EXT) && key & 0xff)
    {
        put_char(&kconsole, key & 0xff);
        return;
    }

    int code = key & MASK_RAW;

    switch (code)
    {
    case KEY_UP:
        break;
    case KEY_DOWN:
        break;
    case KEY_ENTER:
        put_char(&kconsole, '\n');
        break;
    case KEY_BACKSPACE:
        put_char(&kconsole, '\b');
        break;
    default:
        break;
    }
}