#include <onix/test.h>
#include <onix/process.h>
#include <onix/stdio.h>
#include <onix/syscall.h>

void test_process_a()
{
    int i = 0;
    while (true)
    {
        get_ticks();
        printf("This is %s - %d : %d\n\0",
               process_ready->name,
               process_ready->pid, i++);
        delay(5000000);
    }

}

void test_process_b()
{
    int i = 0;
    while (true)
    {
        printf("This is %s - %d : %d\n\0",
               process_ready->name,
               process_ready->pid, i++);
        delay(5000000);
    }
}

void test_process_c()
{
    int i = 0;
    while (true)
    {
        printf("This is %s - %d : %d\n\0",
               process_ready->name,
               process_ready->pid, i++);
        delay(5000000);
    }
}
