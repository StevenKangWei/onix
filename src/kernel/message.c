#include <onix/message.h>
#include <onix/string.h>
#include <onix/assert.h>
#include <onix/process.h>

void reset_message(Message *message)
{
    memset(message, 0, sizeof(Message));
}

int retval_message(Message *message)
{
    return message->u.m3.m3i1;
}

int sendrecv(int type, int peer, Message *message) // peer process id
{
    int ret = 0;

    if (type == RECEIVE)
        reset_message(message);

    switch (type)
    {
    case BOTH:
        ret = _sendrecv(SEND, peer, message);
        if (ret == 0)
            ret = _sendrecv(RECEIVE, peer, message);
        break;
    case SEND:
    case RECEIVE:
        ret = _sendrecv(type, peer, message);
        break;
    default:
        assert((type == BOTH) ||
               (type == SEND) || (type == RECEIVE));
        break;
    }
    return ret;
}

int sys_sendrecv(int type, int peer, Message *message, Process *process)
{
    return 12345;
}