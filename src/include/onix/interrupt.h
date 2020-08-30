#ifndef ONIX_INTERRUPT_H
#define ONIX_INTERRUPT_H

#include <onix/mode.h>

#define IDT_SIZE 256

#define INT_VECTOR_IRQ0 0x20
#define INT_VECTOR_IRQ8 0x28

/* 8259A interrupt controller ports. */
#define INT_M_CTL 0x20     /* I/O port for interrupt controller       <Master> */
#define INT_M_CTLMASK 0x21 /* setting bits in this port disables ints <Master> */
#define INT_S_CTL 0xA0     /* I/O port for second interrupt controller<Slave>  */
#define INT_S_CTLMASK 0xA1 /* setting bits in this port disables ints <Slave>  */

/* Hardware interrupts */
#define IRQ_SIZE 16 /* Number of IRQs */
#define CLOCK_IRQ 0
#define KEYBOARD_IRQ 1
#define CASCADE_IRQ 2   /* cascade enable for 2nd AT controller */
#define ETHER_IRQ 3     /* default ethernet interrupt vector */
#define SECONDARY_IRQ 3 /* RS232 interrupt vector for port 2 */
#define RS232_IRQ 4     /* RS232 interrupt vector for port 1 */
#define XT_WINI_IRQ 5   /* xt winchester */
#define FLOPPY_IRQ 6    /* floppy disk */
#define PRINTER_IRQ 7
#define AT_WINI_IRQ 14 /* at winchester */

#define EOI 0x20;

extern Gate idt[IDT_SIZE];
extern Pointer idt_ptr;

typedef void (*irq_handler)(int irq);
extern irq_handler irq_table[];

void init_interrupts();
void init_idt();
void init_idt_desc(uchar vector, u8 desc_type, void *handler, uchar privilege);

extern void hwint00();
extern void hwint01();
extern void hwint02();
extern void hwint03();
extern void hwint04();
extern void hwint05();
extern void hwint06();
extern void hwint07();
extern void hwint08();
extern void hwint09();
extern void hwint10();
extern void hwint11();
extern void hwint12();
extern void hwint13();
extern void hwint14();
extern void hwint15();

extern int interrupt_enter;

void clock_handler(int irq);
void put_irq_handler(int irq, irq_handler handler);

extern void enable_irq(int irq);
extern void disable_irq(int irq);

void hwint_test_handler(int irq);

#endif