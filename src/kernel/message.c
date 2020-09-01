#include <onix/message.h>
#include <onix/string.h>
#include <onix/assert.h>
#include <onix/process.h>
#include <onix/interrupt.h>
#include <onix/memory.h>
#include <onix/process.h>

static int send_message(Process *current, int dest, Message *message);
static int recv_message(Process *current, int dest, Message *message);
static void block(Process *process);
static void unblock(Process *process);
static int deadlock(int src, int dest);

void reset_message(Message *message)
{
    memset(message, 0, sizeof(Message));
}

int get_message_value(Message *message)
{
    return message->u.m3.m3i1;
}

void set_message_value(Message *message, int value)
{
    message->u.m3.m3i1 = value;
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
    assert(interrupt_count == 1); /* make sure we are not in ring0 */
    assert((peer >= 0 && peer < PROCESS_SIZE) || peer == PEER_ANY || peer == PEER_INTERRUPT);

    int ret = 0;
    int caller = process->pid;

    Message *mla = (Message *)va2la(process, message);
    mla->source = caller;

    assert(mla->source != peer);

    /**
	 * Actually we have the third message type: BOTH. However, it is not
	 * allowed to be passed to the kernel directly. Kernel doesn't know
	 * it at all. It is transformed into a SEND followed by a RECEIVE
	 * by `send_recv()'.
	 */
    if (type == SEND)
    {
        ret = send_message(process, peer, message);
        if (ret != 0)
            return ret;
    }
    else if (type == RECEIVE)
    {
        ret = recv_message(process, peer, message);
        if (ret != 0)
            return ret;
    }
    else
    {
        panic("{sys_sendrec} invalid type: "
              "%d (SEND:%d, RECEIVE:%d).",
              type, SEND, RECEIVE);
    }
    return 0;
}

static void block_process(Process *process)
{
    assert(process->flags);
    schedule();
}

static void unblock_process(Process *process)
{
    assert(process->flags == 0);
}


static int deadlock(int src, int dest)
{
    return 0;
}

static int send_message(Process *current, int dest, Message *message)
{
    set_message_value(message, 12345);
}

static int recv_message(Process *current, int src, Message *message)
{
    return 0;
}