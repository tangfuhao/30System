#include "../apilib/syscall.h"

extern "C" {
	void HariMain(void);
}

#define SCREEN_W 200
#define SCREEN_H 200

static int point[16][2] = {
	{ 196, 100 }, { 187,  61 }, { 164,  29 }, { 129,   9 }, {  90,   5 },
	{  53,  17 }, {  23,  44 }, {   7,  81 }, {   7, 119 }, {  23, 156 },
	{  53, 183 }, {  90, 195 }, { 129, 191 }, { 164, 171 }, { 187, 139 },
	{ 196, 100 }
};

static word color[16] = {
	0x0000, 0xf800, 0x07e0, 0xffe0, 0x001f, 0xf81f, 0x07ff, 0xffff, 
	0xc638, 0x8000, 0x0420, 0x8420, 0x0010, 0x8010, 0x0430, 0x8430  
};

void HariMain(void)
{
	byte* buffer = (byte *) api_malloc ((SCREEN_W + 12) * (SCREEN_H + 28) * sizeof(short));
	int window = api_openwin(buffer, SCREEN_W + 12, SCREEN_H + 28, -1, "bball");
	
	/* îwåiÇìhÇËÇ¬Ç‘Ç∑ */
	api_boxfilwin(window, 6, 22, 200, 200, 0 /* çï */);
	
	for (int i = 0; i <= 14; i++) {
		int x0, y0, dis;
		x0 = point[i][0];
		y0 = point[i][1];
		for (int j = i + 1; j <= 15; j++) {
			dis = j - i; /* 2Ç¬ÇÃì_ÇÃãóó£ */
			if (dis >= 8)
			dis = 15 - dis; /* ãtâÒÇËÇ…êîÇ¶ÇÈ */
			if (dis != 0) {
				api_linewin(window, x0 + 6, y0 + 22, point[j][0] + 6, point[j][1] + 22, color[16 - dis]);
			}
		}
	}
	
	/* ïîï™çƒï`âÊ */
	api_refreshwin(window, 6, 22, 206, 222);
	
	/* c ÉLÅ[Ç≈èIóπ */
	while (api_getkey(0) != 'c');
	
    api_closewin(window);
	api_end();
}
