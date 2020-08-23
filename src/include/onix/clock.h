#ifndef ONIX_CLOCK_H
#define ONIX_CLOCK_H

extern int kernel_ticks;
void clock_handler(int irq);

void init_clock();

#endif