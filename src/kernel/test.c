#include <onix/test.h>
#include <onix/type.h>
#include <onix/stdio.h>
#include <onix/time.h>
#include <onix/syscall.h>
#include <onix/console.h>
#include <onix/process.h>

void test_process_a()
{
    int i = 0;
    while (true)
    {
        i++;
        char ch;
        if (i % 2 == 0)
            ch = 'A';
        else
        {
            ch = ' ';
        }
        int delta = kconsole.current - kconsole.start;
        set_char(ch, COLOR_DEFAULT, delta + 75, 0);
        sleep(500);
    }
}

void test_process_b()
{
    int i = 0;
    while (true)
    {
        i++;
        char ch;
        if (i % 2 == 0)
            ch = 'B';
        else
        {
            ch = ' ';
        }
        int delta = kconsole.current - kconsole.start;
        set_char(ch, COLOR_DEFAULT, delta + 77, 0);
        sleep(1500);
    }
}

void test_process_c()
{
    int i = 0;
    while (true)
    {
        i++;
        char ch;
        if (i % 2 == 0)
            ch = 'C';
        else
        {
            ch = ' ';
        }
        int delta = kconsole.current - kconsole.start;
        set_char(ch, COLOR_DEFAULT, delta + 79, 0);
        sleep(3000);
    }
}