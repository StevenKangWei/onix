#ifndef ONIX_KEYBOARD_H
#define ONIX_KEYBOARD_H

#include <onix/queue.h>

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

#define KEYBOARD_BUFFER_SIZE 1024

extern char key_buffer[KEYBOARD_BUFFER_SIZE];

extern Queue keyqueue;

void keyboard_handler(int irq);

void init_keyboard();

typedef void (*callback)(int key);

void read_keyboard(callback);

#endif