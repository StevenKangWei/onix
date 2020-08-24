#include <onix/keyboard.h>
#include <onix/interrupt.h>
#include <onix/stdio.h>

void keyboard_handler(int irq)
{
    printf("Keyboard down....\n\0");
}

void init_keyboard()
{
    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}