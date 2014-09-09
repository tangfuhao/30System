#include "syscall.h"

void api_closewin(int win)
{
	dword result;
	SYSCALL_1(API_CLOSEWIN, result, win);
}
