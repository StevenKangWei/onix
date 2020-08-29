#include <onix/kernel.h>
#include <onix/mode.h>
#include <onix/console.h>
#include <onix/stdio.h>
#include <onix/time.h>

u32 KERNEL_STACK[KERNEL_STACK_SIZE];
void *KERNEL_STACK_TOP = KERNEL_STACK + KERNEL_STACK_SIZE;

void init_kernel()
{
    save_gdt(&gdt_ptr);
    init_gdt();
    load_gdt(&gdt_ptr);

    init_console();
    clear(&kconsole);

    int i = 1;
    while (true)
    {
        if (i > 103)
            continue;
        kprintf("Hello, Onix!!! %d\n\0", i++);
        kprintf("cursor %d current %d start %d limit %d\n\0",
                kconsole.cursor, kconsole.current, kconsole.start, kconsole.limit);
        delay(500000);
    }
}

void enter_kernel()
{
}
