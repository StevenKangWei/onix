void __kernel_entry()
{
    _init_kernel();
}

#include <onix/stdio.h>

void _init_kernel()
{
    clear();
    char *string = "Hello, Onix!!!\n\a\0";
    printf(string);
}