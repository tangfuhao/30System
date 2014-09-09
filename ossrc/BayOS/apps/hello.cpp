#include "../apilib/syscall.h"

extern "C" {
	void HariMain(void);
}

void HariMain(void)
{
	api_putchar('H');
	api_putchar('i');
	api_putchar('!');
	api_putchar('\n');
	api_putstr0("Hello,World!\n");
	api_putstr1("How are you?\n", 13);
	api_end();
}
