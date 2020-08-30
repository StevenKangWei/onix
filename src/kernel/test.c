#include <onix/test.h>
#include <onix/type.h>
#include <onix/stdio.h>
#include <onix/time.h>
#include <onix/syscall.h>
#include <onix/process.h>

void test_process_a()
{
    int i = 0;
    int ticks = 0;
    while (true)
    {
        ticks = get_ticks();
        kprintf("This is process A %d %d %d\n\0", i++, ticks, process_ready->pid);
        delay(1000);
    }
}

void test_process_b()
{
    int i = 0;
    while (true)
    {
        kprintf("This is process B %d\n\0", i++);
        delay(1500);
    }
}

void test_process_c()
{
    int i = 0;
    while (true)
    {
        kprintf("This is process C %d\n\0", i++);
        delay(3000);
    }
}