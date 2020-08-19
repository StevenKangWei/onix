void __kernel_entry()
{
    _init_kernel();
}

#include <onix/stdio.h>

void _init_kernel()
{
    set_cursor(0, 0);
    char *string = "Hello, Onix!!!\0";
    printf(string);
}