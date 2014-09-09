#include "syscall.h"

void api_putstrwin(int win, int x, int y, int col, int len, char* str)
{
	dword result;
	int x_y = (x << 16) | y;
	SYSCALL_4(API_PUTSTRWIN, result, win, x_y, col, str);
}
