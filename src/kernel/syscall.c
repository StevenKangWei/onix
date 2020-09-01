#include <onix/syscall.h>
#include <onix/clock.h>
#include <onix/string.h>
#include <onix/assert.h>
#include <onix/stdio.h>
#include <onix/console.h>
#include <onix/time.h>

syscall syscall_table[SYSCALL_SIZE] = {sys_pause, sys_get_ticks, sys_sendrecv};

int sys_get_ticks()
{
    return kernel_ticks;
}

void task_syscall()
{
    int i = 0;
    while (true)
    {
        i++;
        char ch;
        if (i % 2 == 0)
            ch = 'S';
        else
        {
            ch = ' ';
        }
        int delta = kconsole.current - kconsole.start;
        set_char(ch, COLOR_DEFAULT, delta + 73, 0);
        sleep(3000);
    }
}

int test_syscall()
{
    Message message;
    reset_message(&message);
    message.type = TEST_CALL;
    sendrecv(BOTH, PEER_SYSCALL, &message);
    return get_message_value(&message);
}