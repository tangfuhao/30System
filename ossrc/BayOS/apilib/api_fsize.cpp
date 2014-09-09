#include "syscall.h"

int api_fsize(int fhandle, int mode)
{
	dword result;
	SYSCALL_2(API_FSIZE, result, fhandle, mode);
	return result;
}
