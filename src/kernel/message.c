#include <onix/message.h>
#include <onix/string.h>
#include <onix/assert.h>
#include <onix/process.h>
#include <onix/interrupt.h>
#include <onix/memory.h>
#include <onix/process.h>

static int send_message(Process *current, int peer, Message *message);
static int recv_message(Process *current, int peer, Message *message);

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

void block(Process *process)
{
    assert(process->flags);
    schedule();
}

void unblock(Process *process)
{
    assert(process->flags == 0);
}

int deadlock(int src, int dest)
{
    Process *process = process_table + dest;
    while (true)
    {
        if (!(process->flags & PROCESS_SENDING))
            return 0;
        if (process->sendto == src)
        {
            /* print the chain */
            process = process_table + dest;
            kprintf("deadlock =_=%s \n\0", process->name);
            do
            {
                assert(process->message);
                process = process_table + process->sendto;
                kprintf("deadlock ->%s \n\0", process->name);
            } while (process != process_table + src);
            kprintf("deadlock =_= \n\0");
            return 1;
        }
        process = process_table + process->sendto;
    }
    return 0;
}

int send_message(Process *current, int peer, Message *message)
{
    Process *sender = current;
    Process *receiver = process_table + peer; /* proc peer */

    assert(sender->pid != peer);

    /* check for deadlock here */
    if (deadlock(sender->pid, peer))
    {
        panic(">>DEADLOCK<< %s->%s", sender->name, receiver->name);
    }

    if ((receiver->flags & PROCESS_RECEIVING) && /* peer is waiting for the message */
        (receiver->recvfrom == sender->pid ||
         receiver->recvfrom == PEER_ANY))
    {
        assert(receiver->message);
        assert(message);

        memcpy(va2la(receiver, receiver->message), va2la(sender, message), sizeof(Message));

        receiver->message = 0;
        receiver->flags &= ~PROCESS_RECEIVING; /* peer has received the message */
        receiver->recvfrom = PEER_NONE;

        unblock(receiver);

        assert(receiver->flags == 0);
        assert(receiver->message == 0);
        assert(receiver->recvfrom == PEER_NONE);
        assert(receiver->sendto == PEER_NONE);
        assert(sender->flags == 0);
        assert(sender->message == 0);
        assert(sender->recvfrom == PEER_NONE);
        assert(sender->sendto == PEER_NONE);
    }
    else
    { /* peer is not waiting for the message */
        sender->flags |= PROCESS_SENDING;
        assert(sender->flags == PROCESS_SENDING);
        sender->sendto = peer;
        sender->message = message;

        /* append to the sending queue */
        Process *process;
        if (receiver->sending)
        {
            process = receiver->sending;
            while (process->next)
                process = process->next;
            process->next = sender;
        }
        else
        {
            receiver->sending = sender;
        }
        sender->next = 0;

        block(sender);

        assert(sender->flags == PROCESS_SENDING);
        assert(sender->message != 0);
        assert(sender->recvfrom == PEER_NONE);
        assert(sender->sendto == peer);
    }
    return 0;
}

int recv_message(Process *current, int src, Message *message)
{
    Process *recv = current;
    Process *from = 0; /* from which the message will be fetched */
    Process *prev = 0;

    int copyok = 0;

    assert(recv->pid != src);

    if ((recv->interrupt_busy) && ((src == PEER_ANY) || (src == PEER_INTERRUPT)))
    {
        /* There is an interrupt needs recv's handling and
        * recv is ready to handle it.
        */

        Message msg;
        reset_message(&msg);
        msg.source = PEER_INTERRUPT;
        msg.type = HARD_INT;

        assert(message);

        memcpy(va2la(recv, message), &msg, sizeof(Message));

        recv->interrupt_busy = 0;

        assert(recv->flags == 0);
        assert(recv->message == 0);
        assert(recv->sendto == PEER_NONE);
        assert(recv->interrupt_busy == 0);

        return 0;
    }

    /* Arrives here if no interrupt for recv. */
    if (src == PEER_ANY)
    {
        /* recv is ready to receive messages from
		 * ANY proc, we'll check the sending queue and pick the
		 * first proc in it.
		 */
        if (recv->sending)
        {
            from = recv->sending;
            copyok = 1;

            assert(recv->flags == 0);
            assert(recv->message == 0);
            assert(recv->recvfrom == PEER_NONE);
            assert(recv->sendto == PEER_NONE);
            assert(recv->sending != 0);
            assert(from->flags == PROCESS_SENDING);
            assert(from->message != 0);
            assert(from->recvfrom == PEER_NONE);
            assert(from->sendto == recv->pid);
        }
    }
    else if (src >= 0 && src < PROCESS_SIZE)
    {
        /* recv wants to receive a message from
        * a certain proc: src.
        */
        from = &process_table[src];

        if ((from->flags & PROCESS_SENDING) &&
            (from->sendto == recv->pid))
        {
            /* Perfect, src is sending a message to
			 * recv.
			 */
            copyok = 1;

            Process *process = recv->sending;

            assert(process); /* from must have been appended to the
                        * queue, so the queue must not be NULL
                        */

            while (process)
            {
                assert(from->flags & PROCESS_SENDING);

                if (process->pid == src) /* if process is the one */
                    break;

                prev = process;
                process = process->next;
            }

            assert(recv->flags == 0);
            assert(recv->message == 0);
            assert(recv->recvfrom == PEER_NONE);
            assert(recv->sendto == PEER_NONE);
            assert(recv->sending != 0);
            assert(from->flags == PROCESS_SENDING);
            assert(from->message != 0);
            assert(from->recvfrom == PEER_NONE);
            assert(from->sendto == recv->pid);
        }
    }

    if (copyok)
    {
        /* It's determined from which proc the message will
		 * be copied. Note that this proc must have been
		 * waiting for this moment in the queue, so we should
		 * remove it from the queue.
		 */
        if (from == recv->sending)
        { /* the 1st one */
            assert(prev == 0);
            recv->sending = from->next;
            from->next = 0;
        }
        else
        {
            assert(prev);
            prev->next = from->next;
            from->next = 0;
        }

        assert(message);
        assert(from->message);

        /* copy the message */
        memcpy(va2la(recv, message),
               va2la(from, from->message),
               sizeof(Message));

        from->message = 0;
        from->sendto = PEER_NONE;
        from->flags &= ~PROCESS_SENDING;
        unblock(from);
    }
    else
    { /* nobody's sending any msg */
        /* Set flags so that recv will not
		 * be scheduled until it is unblocked.
		 */
        recv->flags |= PROCESS_RECEIVING;

        recv->message = message;
        recv->recvfrom = src;

        block(recv);

        assert(recv->flags == PROCESS_RECEIVING);
        assert(recv->message != 0);
        assert(recv->recvfrom != PEER_NONE);
        assert(recv->sendto == PEER_NONE);
        assert(recv->interrupt_busy == 0);
    }

    return 0;
}

void dump_message(const char *title, Message *message)
{
    int packed = 0;
    kprintf(
        "{%s}<0x%x>{%ssrc:%s(%d),%stype:%d,%s(0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x)%s}%s", //, (0x%x, 0x%x, 0x%x)}",
        title,
        (int)message,
        packed ? "" : "\n        ",
        process_table[message->source].name,
        message->source,
        packed ? " " : "\n        ",
        message->type,
        packed ? " " : "\n        ",
        message->u.m3.m3i1,
        message->u.m3.m3i2,
        message->u.m3.m3i3,
        message->u.m3.m3i4,
        (int)message->u.m3.m3p1,
        (int)message->u.m3.m3p2,
        packed ? "" : "\n",
        packed ? "" : "\n" /* , */
    );
}