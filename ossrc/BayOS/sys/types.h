#ifndef __TYPES_H__
#define __TYPES_H__

#define NULL		0
#define INT_MAX		(+0x7fffffff)
#define UINT_MAX	(+0xffffffff)
#define ULONG_MAX	UINT_MAX

#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ unsigned long
#endif
typedef __SIZE_TYPE__ size_t;

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned int   dword;

#define get_word(b) (word)( ((b)[0]<<8)|(b)[1] )
#define get_dword(b) (dword)( (dword)((b)[0])<<24 | (dword)((b)[1])<<16 | (dword)((b)[2])<<8 | (dword)((b)[3]) )

#endif
