#include "syscall.h"

void api_end()
{
	dword result;
	SYSCALL_0(API_END, result);
}
