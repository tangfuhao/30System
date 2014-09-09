#if !defined(_BAYOS_DRIVER_BEEP_H_INCLUDED_)
#define _BAYOS_DRIVER_BEEP_H_INCLUDED_

namespace driver
{
    /* ビープドライバー */
    class Beep
    {
    public:
        static void tone(int freq);
    };
}

#endif
