#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/mode.h>
#include <onix/stdio.h>
#include <onix/console.h>
#include <onix/syscall.h>

Gate idt[IDT_SIZE];
Pointer idt_ptr;
int interrupt_enter;
irq_handler irq_table[IRQ_SIZE];

void init_pic()
{
    kprintf("Initializing PIC...\n\0");
    /* Master 8259, ICW1. */
    out_byte(INT_M_CTL, 0x11);

    /* Slave  8259, ICW1. */
    out_byte(INT_S_CTL, 0x11);

    /* Master 8259, ICW2. */
    out_byte(INT_M_CTLMASK, INT_VECTOR_IRQ0);

    /* Slave  8259, ICW2. */
    out_byte(INT_S_CTLMASK, INT_VECTOR_IRQ8);

    /* Master 8259, ICW3. */
    out_byte(INT_M_CTLMASK, 0x4);

    /* Slave  8259, ICW3. */
    out_byte(INT_S_CTLMASK, 0x2);

    /* Master 8259, ICW4. */
    out_byte(INT_M_CTLMASK, 0x1);

    /* Slave  8259, ICW4. */
    out_byte(INT_S_CTLMASK, 0x1);

    /* Master 8259, OCW1.  */
    out_byte(INT_M_CTLMASK, 0xFF);

    /* Slave  8259, OCW1.  */
    out_byte(INT_S_CTLMASK, 0xFF);
}

void init_interrupts()
{
    kprintf("Initializing Interrupts...\n\0");

    init_pic();

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

    init_idt_desc(INT_VECTOR_SYS_CALL, DA_386IGate, _syscall, PRIVILEGE_USER);

    idt_ptr.limit = IDT_SIZE * sizeof(Gate) - 1;
    idt_ptr.base = (u32)&idt;
    interrupt_enter = 0;

    for (size_t i = 0; i < IRQ_SIZE; i++)
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

void hwint_test_handler(int irq)
{
    set_char('T', COLOR_DEFAULT, 79, 0);
    // kprintf("Test interrupt request %d \n\0", irq);
}

void put_irq_handler(int irq, irq_handler handler)
{
    disable_irq(irq);
    irq_table[irq] = handler;
}

void enable_irq(int irq)
{
    if (irq < 8)
        out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) & ~(1 << irq));
    else
        out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) & ~(1 << irq));
}

void disable_irq(int irq)
{
    if (irq < 8)
        out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) | (1 << irq));
    else
        out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) | (1 << irq));
}