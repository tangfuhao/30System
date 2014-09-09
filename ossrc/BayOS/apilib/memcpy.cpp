#include "syscall.h"

void* memcpy(void* s1, const void* s2, size_t size)
{
	asm volatile("movl %0, %%edi \n"
	             "movl %1, %%esi \n"
	             "movl %2, %%ecx \n"
	             "cld            \n"
	             "rep            \n"
	             "movsb          \n"
	             : /* no output */
	             : "m"(s1), "m"(s2), "m"(size)
	             : "edi", "esi", "ecx");

	return s1;
}
