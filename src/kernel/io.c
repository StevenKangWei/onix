#include <onix/io.h>

// use the assembly implementation
// void io_outb(u16 port, u8 val)
// {
// #ifdef ONIX_DEBUG
//     return;
// #endif
//     asm volatile("outb %0, %1"
//                  :
//                  : "a"(val), "Nd"(port));
// }

// u8 io_inb(u16 port)
// {
// #ifdef ONIX_DEBUG
//     return 0;
// #endif
//     u8 ret;
//     asm volatile("inb %1, %0"
//                  : "=a"(ret)
//                  : "Nd"(port));
//     return ret;
// }