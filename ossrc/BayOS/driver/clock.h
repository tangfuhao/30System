#if !defined(_BAYOS_DRIVER_CLOCK_H_INCLUDED_)
#define _BAYOS_DRIVER_CLOCK_H_INCLUDED_

#define bcd_to_num(c) ((c >> 4) * 10 + (c & 0x0f))

namespace driver
{
    /* リアルタイムクロックドライバー */
    class RTC
    {
    public:
        static int year, month, day, hour, minutes, second;

    public:
        static void init();
    };
}

#endif
