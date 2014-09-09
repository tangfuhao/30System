#include "syscall.h"

void api_putchar(char c)
{
	dword result;
	SYSCALL_1(API_PUTCHAR, result, c);
}
