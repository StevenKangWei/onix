#include <onix/exception.h>
#include <onix/interrupt.h>
#include <onix/stdio.h>

void init_exceptions()
{
    kprintf("Initializing Exceptions...\n\0");

    init_idt_desc(INT_VECTOR_DIVIDE, DA_386IGate, divide_error, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_DEBUG, DA_386IGate, single_step_exception, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_NMI, DA_386IGate, nmi, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_BREAKPOINT, DA_386IGate, breakpoint_exception, PRIVILEGE_USER);
    init_idt_desc(INT_VECTOR_OVERFLOW, DA_386IGate, overflow, PRIVILEGE_USER);
    init_idt_desc(INT_VECTOR_BOUNDS, DA_386IGate, bounds_check, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_INVAL_OP, DA_386IGate, inval_opcode, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_COPROC_NOT, DA_386IGate, copr_not_available, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_DOUBLE_FAULT, DA_386IGate, double_fault, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_COPROC_SEG, DA_386IGate, copr_seg_overrun, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_INVAL_TSS, DA_386IGate, inval_tss, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_SEG_NOT, DA_386IGate, segment_not_present, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_STACK_FAULT, DA_386IGate, stack_exception, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_PROTECTION, DA_386IGate, general_protection, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_PAGE_FAULT, DA_386IGate, page_fault, PRIVILEGE_KERNEL);
    init_idt_desc(INT_VECTOR_COPROC_ERR, DA_386IGate, copr_error, PRIVILEGE_KERNEL);
}

void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags)
{
    char *error_messages[] = {
        "#DE Divide Error\0",
        "#DB RESERVED\0",
        "--  NMI Interrupt\0",
        "#BP Breakpoint\0",
        "#OF Overflow\0",
        "#BR BOUND Range Exceeded\0",
        "#UD Invalid Opcode (Undefined Opcode)\0",
        "#NM Device Not Available (No Math Coprocessor)\0",
        "#DF Double Fault\0",
        "    Coprocessor Segment Overrun (reserved)\0",
        "#TS Invalid TSS\0",
        "#NP Segment Not Present\0",
        "#SS Stack-Segment Fault\0",
        "#GP General Protection\0",
        "#PF Page Fault\0",
        "--  (Intel reserved. Do not use.)\0",
        "#MF x87 FPU Floating-Point Error (Math Fault)\0",
        "#AC Alignment Check\0",
        "#MC Machine Check\0",
        "#XF SIMD Floating-Point Exception\0"};

    kprintf("Exception: %s \n EFLAGS: %d CS: %d EIP: %d \n ErrorCode: %x \0",
            error_messages[vec_no],
            eflags,
            cs,
            eip,
            err_code);
}