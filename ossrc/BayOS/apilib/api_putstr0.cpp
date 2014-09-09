#include "syscall.h"

void api_putstr0(char* s)
{
	dword result;
	SYSCALL_1(API_PUTSTR0, result, s);
}
