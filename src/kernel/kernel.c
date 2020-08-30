#include <onix/kernel.h>
#include <onix/mode.h>
#include <onix/console.h>
#include <onix/stdio.h>
#include <onix/time.h>
#include <onix/interrupt.h>
#include <onix/exception.h>
#include <onix/io.h>
#include <onix/process.h>

u32 KERNEL_STACK[KERNEL_STACK_SIZE];
void *KERNEL_STACK_TOP = KERNEL_STACK + KERNEL_STACK_SIZE;

void init_kernel()
{
    init_console();
    clear(&kconsole);
    kprintf("Initializing Onix...\n\0");

    save_gdt(&gdt_ptr);
    init_gdt();
    load_gdt(&gdt_ptr);

    init_exceptions();
    init_interrupts();
    load_idt(&idt_ptr);

    init_tss();
    init_ldt();

    enable_interrupt();
}

void enter_kernel()
{
    kprintf("Hello, Onix!!!\n\0");
    init_processes();
}
