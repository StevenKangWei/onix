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
        tick = get_ticks();
        printf("This is process A %d: %d %d\n\0", process_ready->pid, i++, tick);
        delay(5000000);
    }
}

void test_process_b()
{
    int i = 0;
    while (true)
    {
        printf("This is process B %d: %d\n\0", process_ready->pid, i++);
        delay(5000000);
    }
}

void test_process_c()
{
    int i = 0;
    while (true)
    {
        printf("This is process C %d: %d\n\0", process_ready->pid, i++);
        delay(5000000);
    }
}
