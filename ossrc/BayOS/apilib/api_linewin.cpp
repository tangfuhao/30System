#include "syscall.h"

void api_linewin(int win, int x0, int y0, int x1, int y1, int col)
{
	dword result;
	int x0_y0 = (x0 << 16) | y0;
	int x1_y1 = (x1 << 16) | y1;
	SYSCALL_4(API_LINEWIN, result, win, x0_y0, x1_y1, col);
}
