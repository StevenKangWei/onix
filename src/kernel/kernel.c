void _init_kernel();
void __kernel_entry()
{
    _init_kernel();
}

#include <onix/stdio.h>

void _init_kernel()
{
#ifndef ONIX_DEBUG
    clear();
#endif
    printf("Hello, Onix!!!\n\0");
}

int main(int argc, char const *argv[])
{
    __kernel_entry();
    return 0;
}
