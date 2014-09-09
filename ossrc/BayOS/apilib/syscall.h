#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define SYSCALL_0(syscall_number, result)                                         \
    asm volatile("movl $%c1, %%ebx \n"                                            \
                 "int  $0x40       \n"                                            \
                 "movl %%eax, %0   \n"                                            \
                 :"=m"(result)                                                    \
                 :"g"(syscall_number)                                             \
                 :"ebx"                                                           \
                 );                                                               \

#define SYSCALL_1(syscall_number, result, arg1)                                   \
    asm volatile("movl $%c1, %%ebx \n"                                            \
                 "movl %2  , %%esi \n"                                            \
                 "int  $0x40       \n"                                            \
                 "movl %%eax, %0   \n"                                            \
                 :"=m"(result)                                                    \
                 :"g"(syscall_number), "m"(arg1)                                  \
                 :"ebx", "esi"                                                    \
                 );                                                               \

#define SYSCALL_2(syscall_number, result, arg1, arg2)                             \
    asm volatile("movl $%c1, %%ebx \n"                                            \
                 "movl %2  , %%esi \n"                                            \
                 "movl %3  , %%ecx \n"                                            \
                 "int  $0x40       \n"                                            \
                 "movl %%eax, %0   \n"                                            \
                 :"=m"(result)                                                    \
                 :"g"(syscall_number), "m"(arg1), "m"(arg2)                       \
                 :"ebx", "esi", "ecx"                                             \
                 );                                                               \

#define SYSCALL_3(syscall_number, result, arg1, arg2, arg3)                       \
    asm volatile("movl $%c1, %%ebx \n"                                            \
                 "movl %2  , %%esi \n"                                            \
                 "movl %3  , %%ecx \n"                                            \
                 "movl %4  , %%edi \n"                                            \
                 "int  $0x40       \n"                                            \
                 "movl %%eax, %0   \n"                                            \
                 :"=m"(result)                                                    \
                 :"g"(syscall_number), "m"(arg1), "m"(arg2), "m"(arg3)            \
                 :"ebx", "esi", "ecx", "edi"                                      \
                 );                                                               \

#define SYSCALL_4(syscall_number, result, arg1, arg2, arg3, arg4)                 \
    asm volatile("movl $%c1, %%ebx \n"                                            \
                 "movl %2  , %%esi \n"                                            \
                 "movl %3  , %%ecx \n"                                            \
                 "movl %4  , %%edi \n"                                            \
                 "movl %5  , %%edx \n"                                            \
                 "int  $0x40       \n"                                            \
                 "movl %%eax, %0   \n"                                            \
                 :"=m"(result)                                                    \
                 :"g"(syscall_number), "m"(arg1), "m"(arg2), "m"(arg3), "m"(arg4) \
                 :"ebx", "esi", "ecx", "edi", "edx"                               \
                 );                                                               \

#define SYSCALL_5(syscall_number, result, arg1, arg2, arg3, arg4, arg5)           \
    asm volatile("movl $%c1, %%ebx \n"                                            \
                 "movl %2  , %%esi \n"                                            \
                 "movl %3  , %%ecx \n"                                            \
                 "movl %4  , %%edi \n"                                            \
                 "movl %5  , %%edx \n"                                            \
                 "movl %6  , %%ebp \n"                                            \
                 "int  $0x40       \n"                                            \
                 "movl %%eax, %0   \n"                                            \
                 :"=m"(result)                                                    \
                 :"g"(syscall_number), "m"(arg1), "m"(arg2), "m"(arg3), "m"(arg4), "m"(arg5) \
                 :"ebx", "esi", "ecx", "edi", "edx", "ebp"                        \
                 );                                                               \

#define SYSCALL_6(syscall_number, result, arg1, arg2, arg3, arg4, arg5, arg6)     \
    asm volatile("movl $%c1, %%ebx \n"                                            \
                 "movl %2  , %%esi \n"                                            \
                 "movl %3  , %%ecx \n"                                            \
                 "movl %4  , %%edi \n"                                            \
                 "movl %5  , %%edx \n"                                            \
                 "movl %6  , %%ebp \n"                                            \
                 "movl %7  , %%esp \n"                                            \
                 "int  $0x40       \n"                                            \
                 "movl %%eax, %0   \n"                                            \
                 :"=m"(result)                                                    \
                 :"g"(syscall_number), "m"(arg1), "m"(arg2), "m"(arg3), "m"(arg4), "m"(arg5), "m"(arg6) \
                 :"ebx", "esi", "ecx", "edi", "edx", "ebp", "esp"                 \
                 );                                                               \

#include "../sys/types.h"

#define API_PUTCHAR    1
#define API_PUTSTR0    2
#define API_PUTSTR1    3
#define API_END        4
#define API_OPENWIN    5
#define API_PUTSTRWIN  6
#define API_BOXFILWIN  7
#define API_MALLOC     8
#define API_FREE       9
#define API_POINT      11
#define API_REFRESHWIN 12
#define API_LINEWIN    13
#define API_CLOSEWIN   14
#define API_GETKEY     15
#define API_BEEP       20
#define API_FOPEN      21
#define API_FCLOSE     22
#define API_FSIZE      24
#define API_FREAD      25

/* はりぼてOS互換システムコール */
extern "C" {
	void  api_putchar(char c);
	void  api_putstr0(char* s);
	void  api_putstr1(char* s, int len);
	void  api_end();
	int   api_openwin(byte* buf, int xsiz, int ysiz, int col_inv, char* title);
	void  api_putstrwin(int win, int x, int y, int col, int len, char* str);
	void  api_boxfilwin(int win, int x0, int y0, int x1, int y1, int col);
	dword api_malloc(int size);
	void  api_free(void* addr);
	void  api_point(int win, int x, int y, int col);
	void  api_refreshwin(int win, int x0, int y0, int x1, int y1);
	void  api_linewin(int win, int x0, int y0, int x1, int y1, int col);
	void  api_closewin(int win);
	int   api_getkey(int mode);
	void  api_beep(int tone);
	int   api_fopen(char* fname);
	void  api_fclose(int fhandle);
	int   api_fsize(int fhandle, int mode);
	int   api_fread(char* buf, int maxsize, int fhandle);
};

/* 独自システムコール */
extern "C" {
	void* memcpy(void* s1, const void* s2, size_t size);
	int printf(const char* format, ...);
};

#endif
