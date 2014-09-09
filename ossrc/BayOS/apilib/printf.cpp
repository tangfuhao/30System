#include "syscall.h"
#include <stdio.h>
#include <string.h>

int printf(const char* format, ...)
{
	static char str[128];
	memset(str, 0, sizeof(str));
	va_list args;
	va_start(args, format);
	int length = vsprintf(str, format, args);
	va_end(args);
	if(length > (int) sizeof(str)) {
		str[sizeof(str) - 1] = 0;
	}
	api_putstr0(str);
	return length;
}
