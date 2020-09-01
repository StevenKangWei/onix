#ifndef ONIX_MESSAGE_H
#define ONIX_MESSAGE_H

#include <onix/type.h>

/* ipc */
#define SEND 1
#define RECEIVE 2
#define BOTH 3 /* BOTH = (SEND | RECEIVE) */

/* Process */
#define SENDING 0x02   /* set when proc trying to send */
#define RECEIVING 0x04 /* set when proc trying to recv */

typedef struct mess1
{
    int m1i1;
    int m1i2;
    int m1i3;
    int m1i4;
} mess1;

typedef struct mess2
{
    void *m2p1;
    void *m2p2;
    void *m2p3;
    void *m2p4;
} mess2;

typedef struct mess3
{
    int m3i1;
    int m3i2;
    int m3i3;
    int m3i4;
    u64 m3l1;
    u64 m3l2;
    void *m3p1;
    void *m3p2;
} mess3;

typedef struct Message
{
    int source;
    int type;
    union
    {
        mess1 m1;
        mess2 m2;
        mess3 m3;
    } u;
} _packed Message;

enum MessageType
{
    /*
    * when hard interrupt occurs, a msg (with type==HARD_INT) will
    * be sent to some tasks
    */
    HARD_INT = 1,

    /* SYS task */
    GET_TICKS,
    TEST_CALL,
};

// message
void reset_message(Message *message);
int get_message_value(Message *message);
void set_message_value(Message *message, int value);

// assembly interrupt
extern int _sendrecv(int type, int peer, Message *message);
// wrapper of _sendrecv
int sendrecv(int type, int peer, Message *message);

#endif