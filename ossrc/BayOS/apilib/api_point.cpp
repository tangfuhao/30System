#include "syscall.h"

void api_point(int win, int x, int y, int col)
{
	dword result;
	SYSCALL_4(API_POINT, result, win, x, y, col);
}
