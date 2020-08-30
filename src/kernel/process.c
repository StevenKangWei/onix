#include <onix/stdio.h>
#include <onix/time.h>
#include <onix/process.h>
#include <onix/string.h>
#include <onix/mode.h>
#include <onix/memory.h>
#include <onix/interrupt.h>
#include <onix/syscall.h>
#include <onix/test.h>

TSS tss;
Process process_table[PROCESS_SIZE];
Process *process_ready;
u32 task_stack[PROCESS_STACK_SIZE_TOTAL];

Task kernel_task_table[KERNEL_TASK_SIZE] = {
    {task_idle, PROCESS_STACK_SIZE, "Process idle\0"},
};

Task user_task_table[USER_TASK_SIZE] = {
    {test_process_a, PROCESS_STACK_SIZE, "Process A\0"},
    {test_process_b, PROCESS_STACK_SIZE, "Process B\0"},
    {test_process_c, PROCESS_STACK_SIZE, "Process C\0"},
};

void init_tss()
{

    kprintf("Initilaizing TSS...\n\0");
    memset(&tss, 0, sizeof(tss));
    tss.ss0 = SELECTOR_DATA;

    kprintf("TTS -> %x:%x\n\0", vir2phys(seg2phys(SELECTOR_DATA), &tss), sizeof(tss) - 1);

    init_descriptor(
        &gdt[GDT_INDEX_TSS],
        vir2phys(seg2phys(SELECTOR_DATA), &tss),
        sizeof(tss) - 1,
        DA_386TSS);
    tss.iobase = sizeof(tss);
}

void init_ldt()
{
    kprintf("Initilaizing LDT...\n\0");

    u16 selector = GDT_INDEX_LDT << 3;
    for (size_t i = 0; i < PROCESS_SIZE; i++)
    {
        init_descriptor(
            &gdt[selector >> 3],
            vir2phys(seg2phys(SELECTOR_DATA), process_table[i].ldt),
            LDT_SIZE * sizeof(Descriptor) - 1,
            DA_LDT);
        selector += 1 << 3;
    }
}

void init_processes()
{
    kprintf("Initializing processes....\n\0");

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
            // privilege = PRIVILEGE_USER;
            // rpl = RPL_USER;
            // eflags = 0x202;

            privilege = PRIVILEGE_TASK;
            rpl = RPL_TASK;
            eflags = 0x1202;
        }

        strcpy(process->name, task->name);
        process->pid = i;

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

        process->frame.eip = (u32)task->init_eip;
        process->frame.esp = (u32)stack;
        process->frame.eflags = eflags;

        kprintf("init process %x task %x %x %x\n\0", process, task->init_eip, stack, eflags);

        stack -= task->stack_size;
        process++;
        selector += 1 << 3;
    }

    process_ready = process_table;

    kprintf("initialized processes!!! %x\n\0", process_ready);
    restart();
}

void restart()
{
    interrupt_enter++;
    _restart();
}

void schedule()
{
    Process *process;
    process_ready++;
    if (process_ready >= process_table + PROCESS_SIZE)
    {
        process_ready = process_table;
    }
}

void task_idle()
{
    while (true)
    {
        kprintf("This is process idle\n\0");
        pause();
        delay(10000000);
    }
}