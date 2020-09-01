#include <onix/syscall.h>
#include <onix/clock.h>
#include <onix/string.h>
#include <onix/assert.h>
#include <onix/stdio.h>
#include <onix/console.h>
#include <onix/time.h>

syscall syscall_table[SYSCALL_SIZE] = {sys_sendrecv, sys_pause, sys_get_ticks};

int sys_get_ticks()
{
    return kernel_ticks;
}

int test_syscall()
{
    Message message;
    reset_message(&message);
    message.type = MESSAGE_TEST_CALL;
    sendrecv(BOTH, PEER_SYSCALL, &message);
    return get_message_value(&message);
}

int get_message_ticks()
{
    Message message;
    reset_message(&message);
    message.type = MESSAG_GET_TICKS;
    sendrecv(BOTH, PEER_SYSCALL, &message);
    return get_message_value(&message);
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
        case MESSAG_GET_TICKS:
            set_message_value(&message, kernel_ticks);
            sendrecv(SEND, message.source, &message);
            break;
        case MESSAGE_TEST_CALL:
            set_message_value(&message, kernel_ticks);
            sendrecv(SEND, message.source, &message);
            break;
        default:
            panic("unknown message type %x from %d\n\0", message.type, message.source);
            break;
        }
    }
}
