#include <onix/test.h>
#include <onix/type.h>
#include <onix/stdio.h>
#include <onix/time.h>

void test_process_a()
{
    int i = 0;
    while (true)
    {
        kprintf("This is process A %d\n\0", i++);
        delay(10000000);
    }
}

void test_process_b()
{
}

void test_process_c()
{
}