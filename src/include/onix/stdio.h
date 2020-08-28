#ifndef ONIX_STDIO_H
#define ONIX_STDIO_H

#include <onix/type.h>

int kprintf(const char *format, ...);
int vsprintf(char *buffer, const char *fmt, va_list args);

#endif