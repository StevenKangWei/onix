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
    Message message;
    int i = 0;
    while (true)
    {
        sendrecv(RECEIVE, PEER_ANY, &message);
        switch (message.type)
        {
        case GET_TICKS:
            set_message_value(&message, kernel_ticks);
            sendrecv(SEND, message.source, &message);
            break;
        case TEST_CALL:
            set_message_value(&message, kernel_ticks);
            sendrecv(SEND, message.source, &message);
            break;
        default:
            panic("unknown message type %x from %d\n\0", message.type, message.source);
            break;
        }
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