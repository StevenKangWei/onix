#include <onix/command.h>
#include <onix/string.h>
#include <onix/stdio.h>
#include <onix/process.h>
#include <onix/memory.h>
#include <onix/syscall.h>
#include <onix/time.h>
#include <onix/console.h>

static const char *COMMAND_CLEAR = "clear\n\0";
static const char *COMMAND_BEEP = "beep\n\0";
static const char *COMMAND_TEST = "test\n\0";

bool execute(const char *command)
{
    if (strcmp(command, COMMAND_CLEAR) == 0)
    {
        command_clear();
        return true;
    }
    if (strcmp(command, COMMAND_BEEP) == 0)
    {
        command_beep();
        return true;
    }
    if (strcmp(command, COMMAND_TEST) == 0)
    {
        command_test();
        return true;
    }
    return false;
}

void command_beep()
{
    kprintf("\a\0");
}

void command_clear()
{
    clear(&kconsole);
}

void command_test()
{
    kprintf("------running test command-----\n\0");
    int test = 10;
    kprintf("virtual address of process %x\n\0", process_ready);

    u32 seg = ldt_seg_linear(process_ready, INDEX_LDT_RW);
    kprintf("ldt data segment of process %x\n\0", seg);

    void *va = va2la(process_ready, &process_ready);
    kprintf("linear address of process %x\n\0", va);

    int n = 100;
    while (n--)
    {
        int result = get_message_ticks();
        kprintf("get message ticks %d \n\0", result);
        delay(100);
    }
}