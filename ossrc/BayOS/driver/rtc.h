#ifndef __RTC_H__
#define __RTC_H__

#define bcd_to_num(c) ((c >> 4) * 10 + (c & 0x0f))

class RTC {
public:
	static int year, month, day, hour, minutes, second;

public:
	static void init();
};

#endif
