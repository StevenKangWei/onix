#ifndef ONIX_PROCESS_H
#define ONIX_PROCESS_H

#include <onix/type.h>
#include <onix/mode.h>

#define LDT_SIZE 2
#define PROCESS_SIZE 1
#define PROCESS_STACK_SIZE 0x8000
#define PROCESS_STACK_SIZE_TOTAL (PROCESS_SIZE * PROCESS_STACK_SIZE)

typedef struct StackFrame
{
    u32 gs;         /* \                                    */
    u32 fs;         /* |                                    */
    u32 es;         /* |                                    */
    u32 ds;         /* |                                    */
    u32 edi;        /* |                                    */
    u32 esi;        /* | pushed by save()                   */
    u32 ebp;        /* |                                    */
    u32 kernel_esp; /* <- 'popad' will ignore it            */
    u32 ebx;        /* |                                    */
    u32 edx;        /* |                                    */
    u32 ecx;        /* |                                    */
    u32 eax;        /* /                                    */
    u32 retaddr;    /* return addr for kernel.asm::save()   */
    u32 eip;        /* \                                    */
    u32 cs;         /* |                                    */
    u32 eflags;     /* | pushed by CPU during interrupt     */
    u32 esp;        /* |                                    */
    u32 ss;         /* /                                    */
} _packed StackFrame;

typedef struct Process
{
    StackFrame frame;         /* process registers saved in stack frame */
    u16 selector;             /* gdt selector giving ldt base and limit */
    Descriptor ldt[LDT_SIZE]; /* local descriptors for code and data */
    u32 pid;                  /* process id passed in from MM */
    char name[16];            /* name of the process */
} Process;

extern Process processes[PROCESS_SIZE];
extern Process *process_ready;

extern char process_stack[PROCESS_STACK_SIZE_TOTAL];

extern void schedule();

void init_processes();

void test_process();
#endif
