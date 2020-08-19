#include <onix/string.h>

size_t strlen(const char *string)
{
    register size_t len = 0;
    while (string[len] != 0)
    {
        len++;
    }
    return len;
}