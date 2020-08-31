#include <onix/io.h>
#include <onix/time.h>

// I/O port     Usage
// 0x40         Channel 0 data port (read/write)
// 0x41         Channel 1 data port (read/write)
// 0x42         Channel 2 data port (read/write)
// 0x43         Mode/Command register (write only, a read is ignored)

// Bits         Usage
// 6 and 7      Select channel :
//                 0 0 = Channel 0
//                 0 1 = Channel 1
//                 1 0 = Channel 2
//                 1 1 = Read-back command (8254 only)
// 4 and 5      Access mode :
//                 0 0 = Latch count value command
//                 0 1 = Access mode: lobyte only
//                 1 0 = Access mode: hibyte only
//                 1 1 = Access mode: lobyte/hibyte
// 1 to 3       Operating mode :
//                 0 0 0 = Mode 0 (interrupt on terminal count)
//                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
//                 0 1 0 = Mode 2 (rate generator)
//                 0 1 1 = Mode 3 (square wave generator)
//                 1 0 0 = Mode 4 (software triggered strobe)
//                 1 0 1 = Mode 5 (hardware triggered strobe)
//                 1 1 0 = Mode 2 (rate generator, same as 010b)
//                 1 1 1 = Mode 3 (square wave generator, same as 011b)
// 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD

#define PS2_WRITE_PORT 0x61

#define INTERNAL_TIMER_CHANNEL0 0x40
#define INTERNAL_TIMER_CHANNEL1 0x41
#define INTERNAL_TIMER_CHANNEL2 0x42
#define INTERNAL_TIMER_MODE 0x43

void play(u32 frequence)
{
    u32 div;
    u8 tmp;

    //Set the PIT to the desired frequency
    div = 1193180 / frequence;

    out_byte(INTERNAL_TIMER_MODE, 0xb6);
    out_byte(INTERNAL_TIMER_CHANNEL2, (u8)(div));
    out_byte(INTERNAL_TIMER_CHANNEL2, (u8)(div >> 8));

    //And play the sound using the PC speaker
    tmp = in_byte(PS2_WRITE_PORT);
    if (tmp != (tmp | 3))
    {
        out_byte(PS2_WRITE_PORT, tmp | 3);
    }
}

//make it shutup
void mute()
{
    u8 tmp = in_byte(PS2_WRITE_PORT) & 0xFC;
    out_byte(PS2_WRITE_PORT, tmp);
}

void beep()
{
    play(1000);
    sleep(200);
    mute();
}