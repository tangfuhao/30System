#if !defined(_BAYOS_DRIVER_KEYBOARD_H_INCLUDED_)
#define _BAYOS_DRIVER_KEYBOARD_H_INCLUDED_

#define PORT_KEYDAT    0x0060
#define PORT_KEYCMD    0x0064
#define PORT_KEYSTA    0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE  0x60
#define KBC_MODE    0x47

#define L_CTRL_PRESSED   29
#define L_SHIFT_PRESSED   42
#define R_SHIFT_PRESSED   54
#define L_ALT_PRESSED   56
#define HOME_PRESSED   71
#define UP_PRESSED    72
#define PAGE_UP_PRESSED   73
#define LEFT_PRESSED   75
#define RIGHT_PRESSED   77
#define END_PRESSED    79
#define DOWN_PRESSED   80
#define PAGE_DOWN_PRESSED  81

#define L_CTRL_RELEASED   157
#define L_SHIFT_RELEASED  170
#define R_SHIFT_RELEASED  182
#define L_ALT_RELEASED   184
#define HOME_RELEASED   199
#define UP_RELEASED    200
#define PAGE_UP_RELEASED  201
#define LEFT_RELEASED   203
#define RIGHT_RELEASED   205
#define END_RELEASED   207
#define DOWN_RELEASED   208
#define PAGE_DOWN_RELEASED  209

namespace driver
{
    /* PS/2キーボードドライバー */
    class Keyboard
    {
    private:
        static char  ascii_table[];
        static char  ascii_shift_table[];

    public:
        static dword shift_ctrl_alt;

    public:
        static void init(void);
        static void wait(void);
        static char decode(int code);
    };
}

#endif
