#include <onix/stdio.h>
#include <onix/time.h>
#include <onix/process.h>
#include <onix/string.h>
#include <onix/mode.h>
#include <onix/memory.h>
#include <onix/interrupt.h>
#include <onix/test.h>

Process process_table[PROCESS_SIZE];
Process *process_ready;
char process_stack[PROCESS_STACK_SIZE_TOTAL];
TSS tss;

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
    init_descriptor(
        &gdt[GDT_INDEX_LDT],
        vir2phys(seg2phys(SELECTOR_DATA), process_table[0].ldt),
        LDT_SIZE * sizeof(Descriptor) - 1,
        DA_LDT);
}

void init_processes()
{
    kprintf("Initializing processes....\n\0");

    Process *process = process_table;
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

    process->frame.eip = (u32)test_process_a;
    process->frame.esp = (u32)(process_stack + PROCESS_STACK_SIZE_TOTAL);
    process->frame.eflags = 0x1202;
    process_ready = process_table;

    kprintf("initialized processes!!! %x\n\0", process_ready);
    restart();
}

void restart()
{
    interrupt_enter++;
    _restart();
}