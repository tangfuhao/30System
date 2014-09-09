#include "syscall.h"

void api_refreshwin(int win, int x0, int y0, int x1, int y1)
{
	dword result;
	int x0_y0 = (x0 << 16 | y0);
	int x1_y1 = (x1 << 16 | y1);
	SYSCALL_3(API_REFRESHWIN, result, win, x0_y0, x1_y1);
}
