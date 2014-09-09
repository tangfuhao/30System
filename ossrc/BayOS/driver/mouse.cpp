#include "../bayos.h"

int    Mouse::x = 0;
int    Mouse::y = 0;
int    Mouse::button = 0;
byte   Mouse::state  = 0;
byte   Mouse::phase  = 0;
char   Mouse::buff[] = { 0, 0, 0 };
Sheet* Mouse::sheet = NULL;
char*  Mouse::cursor[] =
    {
        "1..........",
        "11.........",
        "121........",
        "1221.......",
        "12221......",
        "122221.....",
        "1222221....",
        "12222221...",
        "122222221..",
        "1222222221.",
        "12222211111",
        "1221221....",
        "121.1221...",
        "11..1221...",
        "1....1221..",
        ".....1221..",
        "......11..."
    };

/* マウスの初期化 */
void Mouse::init()
{
    Keyboard::wait();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    Keyboard::wait();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);

    /* 変数の初期化 */
    x = SheetCtl::ctl->xsize / 2;
    y = SheetCtl::ctl->ysize / 2;
    button = 0;
    phase  = 0;

    /* マウス用の下敷きの初期化 */
    sheet = SheetCtl::allocate();
    SheetCtl::setbuf(sheet, (word *) malloc (MOUSE_W * MOUSE_H * sizeof(short)), MOUSE_W, MOUSE_H, LIGHT_YELLOW);
    for (int i = 0; i < MOUSE_W; i++)
    {
        for (int j = 0; j < MOUSE_H; j++)
        {
            if (cursor[j][i] == '1')
            {
                sheet->buffer[j * MOUSE_W + i] = WHITE;
            }
            else if (cursor[j][i] == '2')
            {
                sheet->buffer[j * MOUSE_W + i] = BLACK;
            }
            else
            {
                sheet->buffer[j * MOUSE_W + i] = LIGHT_YELLOW;
            }
        }
    }
    sheet->vx0 = x;
    sheet->vy0 = y;
    SheetCtl::updown(sheet, 2);

    return;
}

/* 生データのデコード */
int Mouse::decode(byte data)
{
    /* マウスの0xfaを待っている段階 */
    if (phase == 0)
    {
        if (data == 0xfa)
        {
            phase = 1;
        }
        return 0;
    }

    /* マウスの1バイト目を待っている段階 */
    if (phase == 1)
    {
        if ((data & 0xc8) == 0x08)
        {
            /* 正しい1バイト目だった */
            buff[0] = data;
            phase   = 2;
        }
        return 0;
    }

    /* マウスの2バイト目を待っている段階 */
    if (phase == 2)
    {
        buff[1] = data;
        phase   = 3;
        return 0;
    }

    /* マウスの3バイト目を待っている段階 */
    if (phase == 3)
    {
        buff[2] = data;
        phase   = 1;
        button  = buff[0] & 0x07;
        int dx  = buff[1];
        int dy  = buff[2];
        if ((buff[0] & 0x10) != 0)
        {
            dx |= 0xffffff00;
        }
        if ((buff[0] & 0x20) != 0)
        {
            dy |= 0xffffff00;
        }
        dy = - dy; /* マウスではy方向の符号が画面と反対 */
        if (dx == 0 && dy == 0)
        {
            if (button == 0)
            {
                state = MOUSE_RELEASED;  /* マウスのボタンを離した */
            }
            else
            {
                state = MOUSE_PRESSED;   /* マウスのボタンを押した */
            }
        }
        else
        {
            if (button == 0)
            {
                state = MOUSE_MOVED;     /* マウスの移動 */
            }
            else
            {
                state = MOUSE_DRAGGED;   /* マウスのドラッグ */
            }
        }
        x += dx;
        y += dy;
        /* 画面の端に行ってしまった場合の処理 */
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x >= SheetCtl::ctl->xsize) x = SheetCtl::ctl->xsize;
        if (y >= SheetCtl::ctl->ysize) y = SheetCtl::ctl->ysize;
        return 1;
    }
    return -1; /* ここに来ることはないはず */
}

/* 描画 */
void Mouse::draw()
{
    SheetCtl::slide(sheet, x, y);
}
