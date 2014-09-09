/* picture.c */
#include "bootpack.h"

int picture_info(struct DLL_STRPICENV *env, int *info, int size, char *fp)
{
	/* �������� */
	if (info_BMP(env, info, size, fp) == 0) {
		/* BMP�ł͂Ȃ����� */
		if (info_JPEG(env, info, size, fp) == 0) {
			/* JPEG�ł��Ȃ����� */
			if (info_ICO(env, info, size, fp) == 0) {
				/* ICO�ł��Ȃ����� */
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
	x &= 1; /* ��������� */
	y &= 1;
	i = table[x + y * 2];	/* ���ԐF����邽�߂̒萔 */
	r = (r * 21) / 256;	/* ����� 0�`20 �ɂȂ� */
	g = (g * 21) / 256;
	b = (b * 21) / 256;
	r = (r + i) / 4;	/* ����� 0�`5 �ɂȂ� */
	g = (g + i) / 4;
	b = (b + i) / 4;
	return 16 + r + g * 6 + b * 36;
}
