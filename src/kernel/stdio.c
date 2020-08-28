#include <onix/stdio.h>
#include <onix/type.h>
#include <onix/console.h>

int vsprintf(char *buffer, const char *format, va_list args)
{
    char *p;
    char tmp[256];
    char *str;
    va_list next = args;

    for (p = buffer; *format; format++)
    {
        if (*format != '%')
        {
            *p++ = *format;
            continue;
        }

        format++;

        switch (*format)
        {
        case 'd':
        case 'i':
            itoa(va_arg(next, int), tmp, 10);
            strcpy(p, tmp);
            p += strlen(tmp);
            break;
        case 'u':
            itoa(va_arg(next, uint), tmp, 10);
            strcpy(p, tmp);
            p += strlen(tmp);
            break;
        case 'o':
            itoa(va_arg(next, uint), tmp, 8);
            strcpy(p, tmp);
            p += strlen(tmp);
            break;
        case 'x':
        case 'X':
            itoa(va_arg(next, int), tmp, 16);
            strcpy(p, tmp);
            p += strlen(tmp);
            break;
        case 'c':
            *(p++) = va_arg(next, char);
            break;
        case 's':
            str = va_arg(next, char *);
            strcpy(p, str);
            p += strlen(str);
            break;
        case '%':
            *(p++) = '%';
            break;
        default:
            break;
        }
    }
    *(p++) = '\0';
    return (p - buffer);
}

int kprintf(const char *format, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, format);
    int count = vsprintf(buffer, format, args);
    int i = 0;
    while (buffer[i])
    {
        put_char(&kconsole, buffer[i++]);
    }
    return i;
}