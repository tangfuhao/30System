#include "apilib.h"

void HariMain(void)
{
	short *buf;
	int win;

	api_initmalloc();
	buf = api_malloc(300 * 70 * 2);
	win = api_openwin(buf, 300, 70, -1, "hello");
	api_boxfilwin(win, 16, 52, 283, 59, 6 /* êÖêF */);
	api_putstrwinEx(win, 56, 28, 0 /* çï */, 12, 2, "hello, world");
	for (;;) {
		if (api_getkey(1) == 0x0a) {
			break; /* EnterÇ»ÇÁbreak; */
		}
	}
	api_end();
}
