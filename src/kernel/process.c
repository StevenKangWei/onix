#include <onix/stdio.h>
#include <onix/time.h>
#include <onix/process.h>
#include <onix/string.h>
#include <onix/mode.h>
#include <onix/kernel.h>
#include <onix/interrupt.h>
#include <onix/test.h>

Process process_table[PROCESS_SIZE];
Process *process_ready;
u32 task_stack[PROCESS_STACK_SIZE_TOTAL];
Task task_table[PROCESS_SIZE] = {
    {test_process_a, PROCESS_STACK_SIZE, "Process A\0"},
    {test_process_b, PROCESS_STACK_SIZE, "Process B\0"},
    {test_process_c, PROCESS_STACK_SIZE, "Process C\0"}};

void init_processes()
{
    printf("Initializing processes....\n\0");

    Process *process = process_table;
    Task *task = task_table;
    u32 *stack = task_stack + PROCESS_STACK_SIZE_TOTAL;
    u16 selector = SELECTOR_LDT;

    for (size_t i = 1; i <= PROCESS_SIZE; i++)
    {
        strcpy(process->name, task->name);
        process->pid = i;
        process->ticks = process->priority = (PROCESS_SIZE - i + 1) * 100;

        printf("ticks %d priority %d \n\0", process->ticks, process->priority);

        process->selector = selector;
        memcpy(&process->ldt[0], &gdt[SELECTOR_CODE >> 3], sizeof(Descriptor));
        process->ldt[0].attr1 = DA_C | PRIVILEGE_TASK << 5;

        memcpy(&process->ldt[1], &gdt[SELECTOR_DATA >> 3], sizeof(Descriptor));
        process->ldt[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;

        process->frame.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->frame.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->frame.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->frame.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->frame.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
        process->frame.gs = (SELECTOR_VIDEO & SA_RPL_MASK) | RPL_TASK;

        printf("init process %x task %x %x\n\0", process, task->init_eip, stack);

        process->frame.eip = (u32)task->init_eip;
        process->frame.esp = (u32)stack;
        process->frame.eflags = 0x1202;

        stack -= task->stack_size;
        process++;
        task++;
        selector += 1 << 3;
    }

    process_ready = process_table;
    kernel_reenter = 0;

    put_irq_handler(CLOCK_IRQ, clock_handler);
    enable_irq(CLOCK_IRQ);

    running();
}

void running()
{
    kernel_reenter++;
    _running();
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