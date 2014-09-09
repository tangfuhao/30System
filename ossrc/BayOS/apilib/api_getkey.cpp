#include "syscall.h"

int api_getkey(int mode)
{
	dword result;
	SYSCALL_0(API_GETKEY, result);
	return result;
}
