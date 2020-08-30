#ifndef ONIX_KEYBOARD_H
#define ONIX_KEYBOARD_H

#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

void keyboard_handler(int irq);

void init_keyboard();

#endif