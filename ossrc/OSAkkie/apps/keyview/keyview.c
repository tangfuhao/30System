#include <stdio.h>
#include "apilib.h"

#define BPX		8
#define BPY		28

void HariMain(void)
{
	char buf[106 * 52], s[5];
	int win, i;

	win = api_openwin(buf, 90 + 16, 16 + 36, -1, "KeyView");
	api_boxfilwin(win, 8, 28, 90 + BPX, 16 + BPY, 0);

	for (;;) {
		i = api_getkey(1);
		if (i) {
			sprintf(s, "0x%x", i);
			api_boxfilwin(win + 1, BPX, BPY, 40 + BPX, 16 + BPY, 0);
			api_putstrwin(win, BPX, BPY, 7, 10, s);
		//	api_refreshwin(win, BPX, BPY, 40 + BPX, 16 + BPY);
		}
	}
}
