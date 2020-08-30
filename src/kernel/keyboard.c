#include <onix/keyboard.h>
#include <onix/stdio.h>
#include <onix/interrupt.h>

void keyboard_handler(int irq)
{
    kprintf("*\0");
}

void init_keyboard()
{
    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}