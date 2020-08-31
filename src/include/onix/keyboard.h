#ifndef ONIX_KEYBOARD_H
#define ONIX_KEYBOARD_H

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

#define KEYBOARD_BUFFER_SIZE 1024

typedef struct KeyQueue
{
    char *head;
    char *tail;
    int count;
    char buffer[KEYBOARD_BUFFER_SIZE];
} KeyQueue;

extern KeyQueue keyqueue;

void keyboard_handler(int irq);

void init_keyboard();

typedef void (*callback)(char ch);

void read_keyboard(callback cb);

#endif