#ifndef ONIX_PROCESS_H
#define ONIX_PROCESS_H

#include <onix/type.h>
#include <onix/mode.h>
#include <onix/message.h>

#define LDT_SIZE 2
#define KERNEL_TASK_SIZE 3
#define USER_TASK_SIZE 3

#define PROCESS_SIZE (KERNEL_TASK_SIZE + USER_TASK_SIZE)
#define PROCESS_STACK_SIZE 0x4000
#define PROCESS_STACK_SIZE_TOTAL (PROCESS_SIZE * PROCESS_STACK_SIZE)

#define PEER_INVALID_DRIVER -20
#define PEER_INTERRUPT -10

#define PEER_IDLE 0
#define PEER_TTY 1
#define PEER_SYSCALL 2
#define PEER_ANY (PROCESS_SIZE + 10)
#define PEER_NONE (PROCESS_SIZE + 20)

typedef struct TSS
{
    u32 backlink;
    u32 esp0; /* stack pointer to use during interrupt */
    u32 ss0;  /*   "   segment  "  "    "        "     */
    u32 esp1;
    u32 ss1;
    u32 esp2;
    u32 ss2;
    u32 cr3;
    u32 eip;
    u32 flags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldt;
    u16 trap;
    u16 iobase;
} _packed TSS; // Task State Segment

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

    int ticks;
    int priority;

    int flags;

    Message *message;

} Process;

typedef void (*Target)();

typedef struct Task
{
    Target init_eip;
    int stack_size;
    char name[32];
    int priority;
} _packed Task;

extern TSS tss;

extern Process process_table[PROCESS_SIZE];
extern Process *process_ready;
extern Task kernel_task_table[];
extern Task user_task_table[];

extern u32 task_stack[PROCESS_STACK_SIZE_TOTAL];

#define process_start process_table
#define process_end (process_table + PROCESS_SIZE)

extern void _restart();
void restart();
void schedule();

void init_tss();
void init_ldt();
void init_processes();

void task_idle();

#endif