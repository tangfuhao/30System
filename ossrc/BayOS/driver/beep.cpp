#include "../bayos.h"

/* Žw’è‚µ‚½Žü”g”‚Ì‰¹‚ð–Â‚ç‚· */
void Beep::tone(int freq)
{
    if (freq == 0)
    {
        int i = io_in8(0x61);
        io_out8(0x61, i & 0x0d);
    }
    else
    {
        int i = 1193180000 / freq;
        io_out8(0x43, 0xb6);
        io_out8(0x42, i & 0xff);
        io_out8(0x42, i >> 8);
        i = io_in8(0x61);
        io_out8(0x61, (i | 0x03) & 0x0f);
    }
}
