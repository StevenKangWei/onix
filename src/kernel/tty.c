#include <onix/tty.h>
#include <onix/type.h>
#include <onix/keyboard.h>

void task_tty()
{
    while (true)
    {
        read_keyboard();
    }
}