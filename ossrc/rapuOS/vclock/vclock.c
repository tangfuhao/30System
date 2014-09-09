#include <stdio.h>
#include <string.h>
#include "apilib.h"

#define	STARS	80

struct DLL_STRPICENV {	/* 64KB */
	int work[64 * 1024 / 4];
};

struct RGB {
	unsigned char b, g, r, t;
};

struct SPACE {
	int x, y, z, col;
};

struct PLANE {
	int x, y, col;
};

struct SYSINFO {
	int scr_x;
	int scr_y;
	int scr_c;
};

/* bmp.nasm */
int info_BMP(struct DLL_STRPICENV *env, int *info, int size, char *fp);
int decode0_BMP(struct DLL_STRPICENV *env, int size, char *fp, int b_type, char *buf, int skip);

int rand(void);		/* 0～32767の範囲で乱数を発生 */
unsigned short rgb2pal(int r, int g, int b, int x, int y, int cb);
int trigono(char tri, int ang, int len);
void error(char *s);

void HariMain(void)
{
	struct DLL_STRPICENV env;
	char   filebuf[512 * 1024], filename[13];
	char   t[7], msg[30];
	short  *buf, *wbuf, *p;
	int    win, i, j, fsize, fpos, info[8];
	int    timer, x, y, snum, mnum, hnum, key;
	struct RGB picbuf[1024 * 768], *q;
	struct SPACE star[STARS];
	struct PLANE lpos[3];
	struct SYSINFO sysinfo;

	/* ファイル読み込み */
	strcpy(filename, "vclock.bmp\0");
	i = api_fopen(filename);
	if (i == 0) {
		error("file open error.\n");
	}
	fsize = api_fsize(i, 0);
	sprintf(msg, "fsize = %d\n", fsize);
	api_putstr0(msg);
	fpos = api_fread(filebuf, fsize, i);
	sprintf(msg, "fpos = %d\n", fpos);
	api_putstr0(msg);
	api_fclose(i);

	/* ファイル内容を画像データに変換 */
	if (info_BMP(&env, info, fsize, filebuf) == 0) {
		error("file type error.\n");
	}
	i = decode0_BMP(&env, fsize, filebuf, 4, (char *) picbuf, 0);
	if (i != 0) {
		error("decode error.\n");
	}
	sprintf(msg, "size = %d, %d\n", info[2], info[3]);
	api_putstr0(msg);

	/* ビットマップの格納 */
	api_sysinfo(&sysinfo);
	api_initmalloc();
	buf = api_malloc(info[2] * info[3] * 2);
	for (i = 0; i < info[3]; i++) {
		p = buf + i * info[2];
		q = picbuf + i * info[2];
		for (j = 0; j < info[2]; j++) {
			p[j] = rgb2pal(q[j].r, q[j].g, q[j].b, j, i, sysinfo.scr_c);
		}
	}
	sprintf(msg, "buf = %d\n", buf);
	api_putstr0(msg);

	for (i = 0; i < STARS; i++) {
		star[i].x   = (rand() % 134) -  67;
		star[i].y   = (rand() % 134) -  67;
		star[i].z   = (rand() % 300) - 600;
		star[i].col = (rand() %   7) +   1;
	}
	lpos[0].col = 5;
	lpos[1].col = 2;
	lpos[2].col = 3;

	/* ウィンドウの作成 */
	wbuf = api_malloc(267 * 267 * 2);
	win  = api_openwin(wbuf, 267, 267, 16, "vclock");
	api_autorefresh(win, 0);

	/* タイマーの初期化 */
	timer = api_alloctimer();
	api_inittimer(timer, 128);

	/* メイン処理 */
	for (;;) {
		if (api_bitblt(win, buf, 0, 0, info[2], info[3]) != 0) {
			api_putstr0("bitblt error.\n");
			break;
		}
		for (i = 0; i < STARS; i++) {
			x = (400 * star[i].x) / (400 - star[i].z);
			y = (400 * star[i].y) / (400 - star[i].z);
			api_point(win, x + 133, y + 133, star[i].col);
		}
		api_systime(t);
		sprintf(msg, "%02d%02d/%02d/%02d\0", t[6], t[5], t[4], t[3]);
		api_putstrwin(win, 93, 166, 4, 10, msg);
		sprintf(msg, "%02d:%02d:%02d\0", t[2], t[1], t[0]);
		api_putstrwin(win, 101, 182, 4, 8, msg);
		snum = (int) t[0];
		mnum = (int) t[1];
		hnum = ((int) t[2] % 12) * 60 + mnum;
		lpos[0].x = trigono('c', 90 - (int) (hnum / 2), 60);
		lpos[0].y = trigono('s', 90 - (int) (hnum / 2), 60);
		lpos[1].x = trigono('c', 90 - mnum * 6,  80);
		lpos[1].y = trigono('s', 90 - mnum * 6,  80);
		lpos[2].x = trigono('c', 90 - snum * 6, 100);
		lpos[2].y = trigono('s', 90 - snum * 6, 100);
		for(i = 0; i < 3; i++) {
			api_linewin(win, 133, 133, 133 + lpos[i].x, 133 - lpos[i].y, lpos[i].col);
		}
		api_refreshwin(win, 0, 0, 267, 267);

		for (i = 0; i < STARS; i++) {
			star[i].z += 30;
			if (star[i].z > 0) {
				star[i].x   = (rand() % 134) -  67;
				star[i].y   = (rand() % 134) -  67;
				star[i].z   = (rand() % 300) - 600;
				star[i].col = (rand() %   7) +   1;
			}
		}

		api_settimer(timer, 5);
		for (;;) {
			key = api_getkey(1);
			if (key == 128 || key == 0x0a) { break; }
		}
		if (key == 0x0a) { break; }
	}

	/* 終了処理 */
	api_free(buf , info[2] * info[3] * 2);
	api_free(wbuf, info[2] * info[3] * 2);
	api_closewin(win);
	api_end();
}

unsigned short rgb2pal(int r, int g, int b, int x, int y, int cb)
{
	if (cb == 8) {
		static	int table[4] = { 3, 1, 0, 2 };
		int		i;
		x &= 1;					/* 偶数か奇数か */
		y &= 1;
		i = table[x + y * 2];	/* 中間色を作るための定数 */
		r = (r * 21) / 256;		/* これで 0～20 になる */
		g = (g * 21) / 256;
		b = (b * 21) / 256;
		r = (r +  i) /   4;		/* これで 0～5 になる */
		g = (g +  i) /   4;
		b = (b +  i) /   4;
		return((unsigned short) (16 + r + g * 6 + b * 36));
	} else {
		return((unsigned short) (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | (b >> 3)));
	}
}

int trigono(char tri, int ang, int len)
{
	static int table[91] = {     0,   175,   349,   524,   698,
							   872,  1045,  1219,  1392,  1571,
							  1736,  1908,  2079,  2250,  2419,
							  2588,  2756,  2924,  3090,  3256,
							  3420,  3584,  3746,  3907,  4067,
							  4226,  4384,  4540,  4695,  4848,
							  5000,  5150,  5299,  5446,  5592,
							  5736,  5878,  6018,  6157,  6293,
							  6428,  6561,  6691,  6820,  6947,
							  7071,  7193,  7314,  7431,  7547,
							  7660,  7771,  7880,  7986,  8090,
							  8192,  8290,  8387,  8480,  8572,
							  8660,  8746,  8829,  8910,  8988,
							  9063,  9135,  9205,  9272,  9336,
							  9397,  9455,  9511,  9563,  9613,
							  9659,  9703,  9744,  9781,  9816,
							  9848,  9877,  9903,  9925,  9945,
							  9962,  9976,  9986,  9994,  9998,
							 10000 };
	char sgn = 1;

	if (tri == 'c') {
		ang += 90;
	}
	ang %= 360;
	if (ang < 0) {
		ang *= -1;
		sgn = -1;
	}
	if (90 < ang && ang <= 180) {
		ang = 180 - ang;
	} else if (180 < ang && ang <= 270) {
		ang -= 180;
		sgn *= -1;
	} else if (270 < ang && ang < 360) {
		ang = 360 - ang;
		sgn *= -1;
	}

	return (int) (sgn * len * table[ang] / 10000);
}

void error(char *s)
{
	api_putstr0(s);
	api_end();
}
