#include <onix/assert.h>
#include <onix/stdio.h>
#include <onix/syscall.h>

void spin(char *name)
{
    kprintf("\nspinning in %s ...\n\0", name);
    while (1)
    {
        pause();
    }
}

void assertion_failure(char *exp, char *file, char *base_file, int line)
{
    kprintf(
        "%c \n--> assert(%s) failed!!!\n"\
        "--> file: %s \n"
        "--> base_file: %s, \n"\
        "--> line %d\n\0",
        MAG_CH_ASSERT,
        exp, file, base_file, line);

    spin("assertion_failure()");

    /* should never arrive here */
    __asm__ __volatile__("ud2");
}

void panic(const char *format, ...)
{
    int i;
    char buf[256];

    /* 4 is the size of fmt in the stack */
    va_list arg = (va_list)((char *)&format + 4);

    i = vsprintf(buf, format, arg);

    kprintf("%c !!panic!! %s", MAG_CH_PANIC, buf);

    /* should never arrive here */
    __asm__ __volatile__("ud2");
}
