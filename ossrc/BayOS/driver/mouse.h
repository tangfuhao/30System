#if !defined(_BAYOS_DRIVER_MOUSE_H_INCLUDED_)
#define _BAYOS_DRIVER_MOUSE_H_INCLUDED_

#define KEYCMD_SENDTO_MOUSE  0xd4
#define MOUSECMD_ENABLE      0xf4
#define MOUSE_W              11
#define MOUSE_H              17
#define MOUSE_RELEASED       0x01
#define MOUSE_PRESSED        0x02
#define MOUSE_MOVED          0x03
#define MOUSE_DRAGGED        0x04

namespace driver
{
    /* PS/2マウスドライバー */
    class Mouse
    {
    public:
        static int    x;
        static int    y;
        static int    button;
        static byte   state;
        static byte   phase;
        static char   buff[];
        static Sheet* sheet;
        static char*  cursor[];

    public:
        static void init();
        static int  decode(byte data);
        static void draw();
    };
}

#endif
