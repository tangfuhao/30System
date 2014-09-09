#ifndef __CONSOLE_H__
#define __CONSOLE_H__

namespace system
{
    /* コンソールクラス */
    class Console {
    public:
        static int    cx;
        static int    cy;
        static int    cw;
        static int    ch;
        static word*  vram;
        static char*  cmd_buf;
        static char*  prt_buf;
        static Sheet* sheet;

    public:
        static void  init();
        static void  locate(int x, int y);
        static void  parseCommand();
        static void  restoreChar();
        static void  drawCarret();
        static void  putPixel(int x, int y, word color);
        static void  putKey(byte c);
        static void  putChar(byte c);
        static void  putString(const char* str);
        static void  printf(const char* format, ...);
    };
}

#endif
