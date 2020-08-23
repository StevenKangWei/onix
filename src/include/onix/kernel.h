#ifndef ONIX_KERNEL_H
#define ONIX_KERNEL_H

#include <onix/type.h>
#define KERNEL_STACK_SIZE 4096

extern uint KERNEL_STACK[KERNEL_STACK_SIZE];
extern u32 KERNEL_STACK_TOP;
extern u32 kernel_reenter;

#endif
