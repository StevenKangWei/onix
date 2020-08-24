#ifndef ONIX_TTY_H
#define ONIX_TTY_H
#include <onix/type.h>

#define CRTC_ADDR_REG 0x3D4 /* CRT Controller Registers - Addr Register */
#define CRTC_DATA_REG 0x3D5 /* CRT Controller Registers - Data Register */
#define START_ADDR_H 0xC    /* reg index of video mem start addr (MSB) */
#define START_ADDR_L 0xD    /* reg index of video mem start addr (LSB) */
#define CURSOR_H 0xE        /* reg index of cursor position (MSB) */
#define CURSOR_L 0xF        /* reg index of cursor position (LSB) */
#define V_MEM_BASE 0xB8000  /* base of color video memory */
#define V_MEM_SIZE 0x8000   /* 32K: B8000H -> BFFFFH */

void task_tty();
void in_process(u32 key);

#endif