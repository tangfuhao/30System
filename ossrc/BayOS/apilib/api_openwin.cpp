#include "syscall.h"

int api_openwin(byte* buf, int xsiz, int ysiz, int col_inv, char* title)
{
	dword result;
	int xsiz_ysiz = (xsiz << 16) | ysiz;
	SYSCALL_4(API_OPENWIN, result, buf, xsiz_ysiz, col_inv, title);
	return result;
}
