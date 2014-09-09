/* picture.c */
#include "bootpack.h"

int picture_info(struct DLL_STRPICENV *env, int *info, int size, char *fp)
{
	/* é©ìÆîªï  */
	if (info_BMP(env, info, size, fp) == 0) {
		/* BMPÇ≈ÇÕÇ»Ç©Ç¡ÇΩ */
		if (info_JPEG(env, info, size, fp) == 0) {
			/* JPEGÇ≈Ç‡Ç»Ç©Ç¡ÇΩ */
			if (info_ICO(env, info, size, fp) == 0) {
				/* ICOÇ≈Ç‡Ç»Ç©Ç¡ÇΩ */
				return 0;
			}
		}
	}
	return 1;
}

int picture_decode0(int mode, struct DLL_STRPICENV *env, int size, char *fp, int b_type, char *buf, int skip)
{
	int i = 1;
	if (mode == 1) {
		i = decode0_BMP (env, size, fp, b_type, (char *) buf, skip);
	} else if (mode == 2) {
		i = decode0_JPEG(env, size, fp, b_type, (char *) buf, skip);
	} else if (mode == 4) {
		i = decode0_ICO (env, size, fp, b_type, (char *) buf, skip);
	}
	return i;
}

unsigned char rgb2pal(int r, int g, int b, int x, int y)
{
	static int table[4] = { 3, 1, 0, 2 };
	int i;
	x &= 1; /* ãÙêîÇ©äÔêîÇ© */
	y &= 1;
	i = table[x + y * 2];	/* íÜä‘êFÇçÏÇÈÇΩÇﬂÇÃíËêî */
	r = (r * 21) / 256;	/* Ç±ÇÍÇ≈ 0Å`20 Ç…Ç»ÇÈ */
	g = (g * 21) / 256;
	b = (b * 21) / 256;
	r = (r + i) / 4;	/* Ç±ÇÍÇ≈ 0Å`5 Ç…Ç»ÇÈ */
	g = (g + i) / 4;
	b = (b + i) / 4;
	return 16 + r + g * 6 + b * 36;
}
