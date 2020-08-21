#include <onix/mode.h>
#include <onix/string.h>
#include <onix/stdio.h>
#include <onix/interrupt.h>

void enter_kernel()
{
    const char string[] = "Hello, Onix!!!\n\0";
    printf(string);
}

void init_kernel()
{
#ifndef ONIX_DEBUG
    clear();
#endif
    const char string[] = "Initializing...\n\0";
    printf(string);
    init_gdt();
    init_interrupts();
}

#ifdef ONIX_DEBUG
int main(int argc, char const *argv[])
{
    __kernel_entry();
    return 0;
}
#endif