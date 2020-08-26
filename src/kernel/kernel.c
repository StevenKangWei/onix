#include <onix/mode.h>
#include <onix/string.h>
#include <onix/stdio.h>
#include <onix/interrupt.h>
#include <onix/process.h>
#include <onix/time.h>
#include <onix/io.h>
#include <onix/kernel.h>
#include <onix/memory.h>
#include <onix/clock.h>
#include <onix/keyboard.h>

u32 KERNEL_STACK[KERNEL_STACK_SIZE];
void *KERNEL_STACK_TOP = KERNEL_STACK + KERNEL_STACK_SIZE;
u32 kernel_reenter;

void init_kernel()
{
#ifndef ONIX_DEBUG
    current_console = NULL;
    clear();
#endif
    printf("Initializing...\n\0");
    printf("KERNEL STACK %x : %x \n\0", KERNEL_STACK, KERNEL_STACK_TOP);

    save_gdt(&gdt_ptr);
    init_gdt();
    init_interrupts();
    init_tss();
    init_ldt();

    load_gdt(&gdt_ptr);
    load_idt(&idt_ptr);
    load_tss(SELECTOR_TSS);
}

void enter_kernel()
{
    printf("Hello, Onix!!!\n\0");

    init_clock();
    init_keyboard();
    init_processes();
    hang();
    int i = 1;
    while (true)
    {
        printf("This is kernel process %d!!! \n\0", i++);
    }
}

#ifdef ONIX_DEBUG
int main(int argc, char const *argv[])
{
    __kernel_entry();
    return 0;
}
#endif

void prit(int a)
{
    printf("%d\n\0", a);
}