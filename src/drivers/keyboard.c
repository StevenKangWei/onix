#include <onix/keyboard.h>
#include <onix/interrupt.h>
#include <onix/stdio.h>

KeyboardInput kinput;

void keyboard_handler(int irq)
{
    io_outb(INT_M_CTL, EOI);

    int status;
    int keycode;
    status = io_inb(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01)
    {
        keycode = io_inb(KEYBOARD_DATA_PORT);
        if (keycode < 0)
            return;
        printf("Key code %x\n\0", keycode);
    }
}

void init_keyboard()
{
    kinput.count = 0;
    kinput.head = kinput.tail = kinput.buf;
    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}