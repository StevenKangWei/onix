#include <onix/tty.h>
#include <onix/type.h>
#include <onix/keyboard.h>
#include <onix/keymap.h>
#include <onix/console.h>
#include <onix/stdio.h>
#include <onix/string.h>

TTY tty;

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
    init_queue(&tty.queue, tty.buffer, TTY_COMMAND_SIZE, sizeof(char));
    print_prompt();
}

void print_prompt()
{
    kprintf("[Onix]# \0");
}

void put_key(char key)
{
    if (key == '\b' && queue_empty(&tty.queue))
    {
        return;
    }
    else if (key == '\b')
    {
        put_char(&kconsole, key);
        popqueue(&tty.queue, &key);
    }
    else
    {
        put_char(&kconsole, key);
        enqueue(&tty.queue, &key);
    }
}

static void execute()
{
    int key = 0;
    if (tty.queue.count > 1)
    {
        enqueue(&tty.queue, &key);
        if (strcmp(tty.buffer, "beep\n\0") == 0)
        {
            kprintf("\a\0");
        }
        else
        {
            kprintf(tty.buffer);
        }
    }
    clear_queue(&tty.queue);
    print_prompt();
}

void key_handler(int key)
{
    if (!(key & FLAG_EXT) && key & 0xff)
    {
        put_key(key & 0xff);
        return;
    }

    int code = key & MASK_RAW;
    int tmp = 0;
    switch (code)
    {
    case KEY_UP:
        break;
    case KEY_DOWN:
        break;
    case KEY_ENTER:
        put_key('\n');
        execute();
        break;
    case KEY_BACKSPACE:
        put_key('\b');
        break;
    default:
        break;
    }
}