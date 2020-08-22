#include <onix/stdio.h>
#include <onix/time.h>
#include <onix/process.h>
#include <onix/string.h>
#include <onix/mode.h>

Process processes[PROCESS_SIZE];
Process *process_ready;
char process_stack[PROCESS_STACK_SIZE_TOTAL];

void test_process()
{
    int i = 0;
    while (true)
    {
        printf("This is a process %d\r\0", i++);
        delay(5000000);
    }
}

void init_processes()
{
    printf("Initializing processes....\n\0");

    Process *process = processes;
    process->selector = SELECTOR_LDT;

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

    process->frame.eip = (u32)test_process;
    process->frame.esp = (u32)(process_stack + PROCESS_STACK_SIZE_TOTAL);
    process->frame.eflags = 0x1202;
    process_ready = processes;

    printf("initialized processes!!! %x\n\0", process_ready);
    schedule();
}