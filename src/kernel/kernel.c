#include <onix/mode.h>
#include <onix/stdio.h>

void __enter_kernel()
{
    const char string[] = "Hello, Onix!!!\n\0";
    printf(string);
}

void __init_kernel()
{
#ifndef ONIX_DEBUG
    clear();
#endif
    const char string[] = "Hello, Onix!!!\n\0";
    printf(string);

    
}

#ifdef ONIX_DEBUG
int main(int argc, char const *argv[])
{
    __kernel_entry();
    return 0;
}
#endif