#if !defined(_BAYOS_H_INCLUDED_)
#define _BAYOS_H_INCLUDED_

#define BAYOS_VERSION "BayOS $Date: 2006/10/29 08:39:16 $"
#define BAYOS_COPYING "Copyright (C) 2004-2006, bayside."

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

#include "sys/List.h"
#include "system/descriptor.h"
#include "system/ihandler.h"
#include "driver/pic.h"
#include "driver/dtk5s.h"
#include "driver/file.h"
#include "system/memory.h"
#include "system/scheduler.h"
//#include "screen.h"
#include "system/sheet.h"
#include "system/console.h"
#include "driver/beep.h"
#include "driver/clock.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"
#include "driver/timer.h"
#include <string.h>

using namespace system;
using namespace driver;

#endif
