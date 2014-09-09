#ifndef __PIT_H__
#define __PIT_H__

/* C言語の関数宣言 */
extern "C" {
	#define PIT_CTRL	0x0043
	#define PIT_CNT0	0x0040
};

/* タイマークラス */
class PIT {
public:
	static dword count;

public:
	static void init(void);
	static void sleep(dword ms);
};

#endif
