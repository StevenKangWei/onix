#include <onix/tty.h>
#include <onix/type.h>
#include <onix/keyboard.h>
#include <onix/keymap.h>
#include <onix/console.h>

void task_tty()
{
    while (true)
    {
        read_keyboard(key_process);
    }
}

void key_process(char key)
{
    if (key & FLAG_EXT)
        return;
    put_char(&kconsole, key);
}