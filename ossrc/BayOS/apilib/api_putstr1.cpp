#include "syscall.h"

void api_putstr1(char* s, int len)
{
	dword result;
	SYSCALL_2(API_PUTSTR1, result, s, len);
}
