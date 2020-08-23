#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/mode.h>
#include <onix/stdio.h>

Gate idt[IDT_SIZE];
DPointer idt_ptr;
irq_handler irq_table[NR_IRQ];

void init_pic()
{
    /* Master 8259, ICW1. */
    io_outb(INT_M_CTL, 0x11);

    /* Slave  8259, ICW1. */
    io_outb(INT_S_CTL, 0x11);

    /* Master 8259, ICW2. */
    io_outb(INT_M_CTLMASK, INT_VECTOR_IRQ0);

    /* Slave  8259, ICW2. */
    io_outb(INT_S_CTLMASK, INT_VECTOR_IRQ8);

    /* Master 8259, ICW3. */
    io_outb(INT_M_CTLMASK, 0x4);

    /* Slave  8259, ICW3. */
    io_outb(INT_S_CTLMASK, 0x2);

    /* Master 8259, ICW4. */
    io_outb(INT_M_CTLMASK, 0x1);

    /* Slave  8259, ICW4. */
    io_outb(INT_S_CTLMASK, 0x1);

    /* Master 8259, OCW1.  */
    // io_outb(INT_M_CTLMASK, 0xFD);
    io_outb(INT_M_CTLMASK, 0xFF);

    /* Slave  8259, OCW1.  */
    io_outb(INT_S_CTLMASK, 0xFF);
}

void init_interrupts()
{
    printf("Initializing Interrupts...\n\0");

    init_pic();

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

    init_idt_desc(INT_VECTOR_IRQ0 + 0, DA_386IGate, hwint00, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ0 + 1, DA_386IGate, hwint01, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ0 + 2, DA_386IGate, hwint02, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ0 + 3, DA_386IGate, hwint03, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ0 + 4, DA_386IGate, hwint04, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ0 + 5, DA_386IGate, hwint05, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ0 + 6, DA_386IGate, hwint06, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ0 + 7, DA_386IGate, hwint07, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ8 + 0, DA_386IGate, hwint08, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ8 + 1, DA_386IGate, hwint09, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ8 + 2, DA_386IGate, hwint10, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ8 + 3, DA_386IGate, hwint11, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ8 + 4, DA_386IGate, hwint12, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ8 + 5, DA_386IGate, hwint13, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ8 + 6, DA_386IGate, hwint14, PRIVILEGE_KERNEL);

    init_idt_desc(INT_VECTOR_IRQ8 + 7, DA_386IGate, hwint15, PRIVILEGE_KERNEL);

    idt_ptr.limite = GDT_SIZE * sizeof(Gate) - 1;
    idt_ptr.base = (u32)&idt;

    for (size_t i = 0; i < NR_IRQ; i++)
    {
        irq_table[i] = hwint_test_handler;
    }
}

void init_idt_desc(uchar vector, u8 desc_type, void *handler, uchar privilege)
{
    Gate *gate = &idt[vector];
    u32 base = (u32)handler;
    gate->offset_low = base & 0xFFFF;
    gate->selector = SELECTOR_CODE;
    gate->dcount = 0;
    gate->attr = desc_type | (privilege << 5);
    gate->offset_high = (base >> 16) & 0xFFFF;
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

    printf("Exception: %s \n EFLAGS: %d CS: %d EIP: %d \n ErrorCode: %x \0",
           error_messages[vec_no],
           eflags,
           cs,
           eip,
           err_code);
}

void hwint_finish()
{
    //EOI
}

void hwint_master_handler(int irq)
{
    // printf("Master interrupt request %d \n\0", irq);

    // // finish interrupt
    int status;
    int keycode;
    io_outb(INT_M_CTL, EOI);
    status = io_inb(KEYBOARD_STATUS_PORT);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01)
    {
        keycode = io_inb(KEYBOARD_DATA_PORT);
        if (keycode < 0)
            return;
        printf("Key code %x\n\0", keycode);
    }
}

void hwint_slave_handler(int irq)
{
    printf("Slave interrupt request %d \n\0", irq);

    // // finish interrupt
    // io_outb(0xA0, 0x20);
    // io_outb(0x20, 0x20);
}

void hwint_test_handler()
{
    putchar('t');
}

void put_irq_handler(int irq, irq_handler handler)
{
    disable_irq(irq);
    irq_table[irq] = handler;
}