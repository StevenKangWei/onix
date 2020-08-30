#include <onix/keyboard.h>
#include <onix/stdio.h>
#include <onix/interrupt.h>
#include <onix/io.h>

void keyboard_handler(int irq)
{
    int status;
    int keycode;

    out_byte(INT_M_CTL, EOI);

    status = in_byte(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01)
    {
        keycode = in_byte(KEYBOARD_DATA_PORT);
        if (keycode < 0)
            return;
        kprintf("Key code %x\n\0", keycode);
    }
}

void init_keyboard()
{
    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}