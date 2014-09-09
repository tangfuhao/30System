#ifndef __Sheet_H__
#define __Sheet_H__

#define BLACK        0x0000
#define LIGHT_RED    0xf800
#define LIGHT_GREEN  0x07e0
#define LIGHT_YELLOW 0xffe0
#define LIGHT_BLUE   0x001f
#define LIGHT_PURPLE 0xf81f
#define LIGHT_AQUA   0x07ff
#define WHITE        0xffff
#define LIGHT_GRAY   0xc638
#define DARK_RED     0x8000
#define DARK_GREEN   0x0420
#define DARK_YELLOW  0x8420
#define DARK_BLUE    0x0010
#define DARK_PURPLE  0x8010
#define DARK_AQUA    0x0430
#define DARK_GRAY    0x8430

#define MAX_SHEETS       256
#define WALLPAPER_WIDTH  640
#define WALLPAPER_HEIGHT 127
#define WALLPAPER_SCALE  2

/* CåæåÍÇÃä÷êîêÈåæ */
extern "C" {
    struct Sheet {
        unsigned short *buffer;
        int width, height, vx0, vy0, transparent, depth, flags;
        struct TASK *task;
    };
    
    struct SHTCTL {
        unsigned char *map;
        unsigned short *vram;
        int xsize, ysize, top;
        struct Sheet *sheets[MAX_SHEETS];
        struct Sheet sheets0[MAX_SHEETS];
    };
    
};

namespace system
{
    /* â∫ï~Ç´ÉNÉâÉX */
    class SheetCtl {
    public:
        static struct SHTCTL *ctl;
        static struct Sheet *back;

    private:
        static void refreshmap(int vx0, int vy0, int vx1, int vy1, int h0);
        static void refreshsub(int vx0, int vy0, int vx1, int vy1, int h0, int h1);

    public:
        static void init();
        static struct Sheet *allocate(/*struct SHTCTL *ctl*/);
        static void setbuf(struct Sheet *sht, unsigned short *buf, int xsize, int ysize, int transparent);
        static void updown(struct Sheet *sht, int depth);
        static void refresh(struct Sheet *sht, int bx0, int by0, int bx1, int by1);
        static void slide(struct Sheet *sht, int vx0, int vy0);
        static void dispose(struct Sheet *sht);
        static void drawPixel(struct Sheet *sht, int x, int y, word col);
        static void drawLine(struct Sheet *sht, int x0, int y0, int x1, int y1, word col);
        static void drawRect(struct Sheet *sht, int x, int y, int width, int height, word col);
        static void fillRect(struct Sheet *sht, int x0, int y0, int x1, int y1, word col);
        static void drawString(struct Sheet *sht, char* title, int x0, int y0);
        static void drawFrame(struct Sheet *sht, char* title);
    };
}

#endif
