#include <onix/tty.h>
#include <onix/type.h>
#include <onix/keyboard.h>
#include <onix/keymap.h>
#include <onix/console.h>
#include <onix/string.h>

char command[TTY_COMMAND_SIZE];

void task_tty()
{
    init_tty();
    while (true)
    {
        read_keyboard(key_process);
    }
}

void init_tty()
{
    memset(command, 0, TTY_COMMAND_SIZE);
}

void key_process(char key)
{
    if (!(key & FLAG_EXT))
    {
        put_char(&kconsole, key);
        return;
    }

    int code = key & MASK_RAW;
    switch (code)
    {
    case UP:
        break;
    case DOWN:
        break;
    case ENTER:
        put_char(&kconsole, '\n');
        break;
    case BACKSPACE:
        put_char(&kconsole, '\b');
        break;
    default:
        break;
    }
}