void __kernel_entry()
{
    _print_kernel();
}

void _print_kernel()
{
    char *string = "Hello, Onix!!!\0";
    int color = 0x04; //black + red
    volatile char *video = (volatile char *)0xB8000;
    while (*string != 0)
    {
        *video++ = *string++;
        *video++ = color;
    }
}