#include "syscall.h"

int api_fread(char* buf, int maxsize, int fhandle)
{
	dword result;
	SYSCALL_3(API_FREAD, result, buf, maxsize, fhandle);
	return result;
}
