#include <onix/io.h>
#include <onix/time.h>

void play(u32 frequence)
{
    u32 div;
    u8 tmp;

    //Set the PIT to the desired frequency
    div = 1193180 / frequence;
    io_outb(0x43, 0xb6);
    io_outb(0x42, (u8)(div));
    io_outb(0x42, (u8)(div >> 8));

    //And play the sound using the PC speaker
    tmp = io_inb(0x61);
    if (tmp != (tmp | 3))
    {
        io_outb(0x61, tmp | 3);
    }
}

//make it shutup
void mute()
{
    u8 tmp = io_inb(0x61) & 0xFC;
    io_outb(0x61, tmp);
}

void beep()
{
    play(1000);
    delay(1500);
    mute();
}