#include "syscall.h"

void api_beep(int tone)
{
	dword result;
	SYSCALL_1(API_PUTCHAR, result, tone);
}
