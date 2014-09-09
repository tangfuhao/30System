#include "syscall.h"

dword api_malloc(int size)
{
	dword result;
	SYSCALL_1(API_MALLOC, result, size);
	return result;
}
