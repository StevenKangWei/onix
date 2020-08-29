#include <onix/kernel.h>
#include <onix/mode.h>
#include <onix/console.h>
#include <onix/stdio.h>
#include <onix/time.h>

u32 KERNEL_STACK[KERNEL_STACK_SIZE];
void *KERNEL_STACK_TOP = KERNEL_STACK + KERNEL_STACK_SIZE;

void init_kernel()
{
    init_console();
    clear(&kconsole);
    kprintf("Hello, Onix!!!\n\0");

    save_gdt(&gdt_ptr);
    init_gdt();
    load_gdt(&gdt_ptr);

    

}

void enter_kernel()
{
}
