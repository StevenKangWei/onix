#include <onix/kernel.h>
#include <onix/mode.h>
#include <onix/console.h>
#include <onix/stdio.h>

u32 KERNEL_STACK[KERNEL_STACK_SIZE];
void *KERNEL_STACK_TOP = KERNEL_STACK + KERNEL_STACK_SIZE;

void init_kernel()
{
    save_gdt(&gdt_ptr);
    init_gdt();
    load_gdt(&gdt_ptr);

    init_console();
    clear(&kconsole);
    kprintf("Hello, world!!!\n\0");
}

void enter_kernel()
{
}
