#ifndef ONIX_MESSAGE_H
#define ONIX_MESSAGE_H

/* Process */
#define SENDING 0x02   /* set when proc trying to send */
#define RECEIVING 0x04 /* set when proc trying to recv */

typedef struct Message
{
    /* data */
} Message;

// message
void reset_message(Message *message);

#endif