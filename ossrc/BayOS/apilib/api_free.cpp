#include "syscall.h"

void api_free(void* addr)
{
	dword result;
	SYSCALL_1(API_FREE, result, addr);
}
