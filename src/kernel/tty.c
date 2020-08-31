#include <onix/tty.h>
#include <onix/type.h>
#include <onix/keyboard.h>
#include <onix/keymap.h>
#include <onix/console.h>
#include <onix/stdio.h>
#include <onix/string.h>

Queue command_queue;
char buffer[TTY_COMMAND_SIZE];

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
    init_queue(&command_queue, buffer, TTY_COMMAND_SIZE, sizeof(char));
    print_prompt();
}

void print_prompt()
{
    kprintf("[Onix]# \0");
}

void put_key(char key)
{
    if (key == '\b' && queue_empty(&command_queue))
    {
        return;
    }
    else if (key == '\b')
    {
        put_char(&kconsole, key);
        popqueue(&command_queue, &key);
    }
    else
    {
        put_char(&kconsole, key);
        enqueue(&command_queue, &key);
    }
}

static void execute()
{
    int key = 0;
    if (command_queue.count > 1)
    {
        enqueue(&command_queue, &key);
        if (strcmp(command_queue.buffer, "beep\0") == 0)
        {
            kprintf("\a\0");
        }
        else
        {
            kprintf(command_queue.buffer);
        }
    }
    clear_queue(&command_queue);
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