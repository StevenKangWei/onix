#include <onix/tty.h>
#include <onix/keyboard.h>

void task_tty()
{
    while (true)
    {
        read_keyboard();
    }
}

void in_process(u32 key)
{
    if (!(key & FLAG_EXT))
    {
        putchar(key & 0xFF);
    }
}