#include <onix/test.h>
#include <onix/process.h>
#include <onix/stdio.h>
#include <onix/syscall.h>

void test_process_a()
{
    int i = 0;
    int tick = 0;
    while (true)
    {
        i++;
        tick = get_ticks();
        char ch;
        if (i % 2 == 0)
            ch = 'A';
        else
        {
            ch = ' ';
        }

        setchar(ch, COLOR_DEFAULT, 75, 0);
        // printf("This is process A %d: %d %d\n\0", process_ready->pid, i++, tick);
        sleep(1000);
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

        setchar(ch, COLOR_DEFAULT, 77, 0);

        // printf("This is process B %d: %d\n\0", process_ready->pid, i++);
        // printf("This\n\0");
        sleep(1000);
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

        setchar(ch, COLOR_DEFAULT, 79, 0);

        // printf("This is process C %d: %d\n\0", process_ready->pid, i++);
        sleep(1000);
    }
}
