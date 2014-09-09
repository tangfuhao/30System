#include "syscall.h"

void api_fclose(int fhandle)
{
	dword result;
	SYSCALL_1(API_FCLOSE, result, fhandle);
}
