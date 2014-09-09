#include "syscall.h"

int api_fopen(char* fname)
{
	dword result;
	SYSCALL_1(API_FOPEN, result, fname);
	return result;
}
