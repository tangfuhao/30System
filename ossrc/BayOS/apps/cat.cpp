#include "../apilib/syscall.h"

extern "C" {
	void HariMain(void);
}

void HariMain(void)
{
	int file = api_fopen("license.txt");
	int size = api_fsize(file /* handle */, 0 /* mode */);
	char* buff = (char *) api_malloc(size + 1);
	api_fread(buff, size, file);
	buff[size] = '\0';
	api_putstr0(buff);
	api_fclose(file);
	api_free(buff);
	api_end();
}
