#include <onix/keyboard.h>
#include <onix/stdio.h>
#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/assert.h>
#include <onix/console.h>

KeyQueue keyqueue;

void keyboard_handler(int irq)
{
    int status = in_byte(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (!(status & 0x01))
        return;

    char code = in_byte(KEYBOARD_DATA_PORT);
    if (code < 0)
        return;
    *(keyqueue.head) = code;

    keyqueue.head++;
    if (keyqueue.head == keyqueue.buffer + KEYBOARD_BUFFER_SIZE)
    {
        keyqueue.head = keyqueue.buffer;
    }
    keyqueue.count++;
    if (keyqueue.head == keyqueue.tail)
    {
        keyqueue.tail++;
        keyqueue.count--;
    }
    if (keyqueue.tail == keyqueue.buffer + KEYBOARD_BUFFER_SIZE)
    {
        keyqueue.tail = keyqueue.buffer;
    }
    assert(keyqueue.count >= 0);
}

void init_keyboard()
{
    keyqueue.count = 0;
    keyqueue.head = keyqueue.tail = keyqueue.buffer;

    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}

void read_keyboard()
{
    if (keyqueue.count <= 0)
        return;

    assert(keyqueue.count > 0);
    char code = *(keyqueue.tail);
    keyqueue.tail++;
    keyqueue.count--;

    if (keyqueue.tail == keyqueue.buffer + KEYBOARD_BUFFER_SIZE)
    {
        keyqueue.tail = keyqueue.buffer;
    }
    put_char(&kconsole, code);
}