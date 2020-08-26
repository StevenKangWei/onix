#include <onix/stdio.h>
#include <onix/time.h>
#include <onix/process.h>
#include <onix/string.h>
#include <onix/mode.h>
#include <onix/kernel.h>
#include <onix/interrupt.h>
#include <onix/tty.h>
#include <onix/syscall.h>
#include <onix/test.h>

Process process_table[PROCESS_SIZE];
Process *process_ready;
u32 task_stack[PROCESS_STACK_SIZE_TOTAL];
Task kernel_task_table[KERNEL_TASK_SIZE] = {
    {task_tty, PROCESS_STACK_SIZE, "Process tty\0"},
};

Task user_task_table[USER_TASK_SIZE] = {
    {test_process_a, PROCESS_STACK_SIZE, "Process A\0"},
    {test_process_b, PROCESS_STACK_SIZE, "Process B\0"},
    {test_process_c, PROCESS_STACK_SIZE, "Process C\0"},
    // {task_idle, PROCESS_STACK_SIZE, "Process idle\0"},
};

void init_processes()
{
    printf("Initializing processes....\n\0");

    Process *process = process_table;
    Task *task;
    u32 *stack = task_stack + PROCESS_STACK_SIZE_TOTAL;
    u16 selector = SELECTOR_LDT;

    u8 privilege;
    u8 rpl;
    int eflags;

    for (size_t i = 1; i <= PROCESS_SIZE; i++)
    {
        if (i <= KERNEL_TASK_SIZE)
        {
            task = kernel_task_table + i - 1;
            privilege = PRIVILEGE_TASK;
            rpl = RPL_TASK;
            eflags = 0x1202;
        }
        else
        {
            task = user_task_table + i - 1 - KERNEL_TASK_SIZE;
            privilege = PRIVILEGE_USER;
            rpl = RPL_USER;
            eflags = 0x202;
        }

        strcpy(process->name, task->name);
        process->pid = i;
        process->ticks = process->priority = (PROCESS_SIZE - i + 1) * 10;

        process->selector = selector;
        memcpy(&process->ldt[0], &gdt[SELECTOR_CODE >> 3], sizeof(Descriptor));
        process->ldt[0].attr1 = DA_C | privilege << 5;

        memcpy(&process->ldt[1], &gdt[SELECTOR_DATA >> 3], sizeof(Descriptor));
        process->ldt[1].attr1 = DA_DRW | privilege << 5;

        process->frame.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->frame.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->frame.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->frame.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->frame.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
        process->frame.gs = (SELECTOR_VIDEO & SA_RPL_MASK) | rpl;

        printf("init process %x task %x %x\n\0", process, task->init_eip, stack);

        process->frame.eip = (u32)task->init_eip;
        process->frame.esp = (u32)stack;
        process->frame.eflags = eflags;

        stack -= task->stack_size;
        process++;
        selector += 1 << 3;
    }

    process_ready = process_table;
    kernel_reenter = 0;

    restart();
}

void restart()
{
    kernel_reenter++;
    _restart();
}

void schedule()
{
    Process *process;
    int max_ticks = 0;

    while (!max_ticks)
    {
        for (size_t i = 0; i < PROCESS_SIZE; i++)
        {
            process = process_table + i;
            if (process->ticks > max_ticks)
            {
                max_ticks = process->ticks;
                process_ready = process;
            }
        }
        if (max_ticks)
        {
            return;
        }
        for (size_t i = 0; i < PROCESS_SIZE; i++)
        {
            process = process_table + i;
            process->ticks = process->priority;
        }
    }
}

void task_idle()
{
    while (true)
    {
        pause();
    }
}