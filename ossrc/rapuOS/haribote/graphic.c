/* グラフィック処理関係 */

#include "bootpack.h"

unsigned short table_8_565[256];

void init_palette(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:黒 */
		0xff, 0x00, 0x00,	/*  1:明るい赤 */
		0x00, 0xff, 0x00,	/*  2:明るい緑 */
		0xff, 0xff, 0x00,	/*  3:明るい黄色 */
		0x00, 0x00, 0xff,	/*  4:明るい青 */
		0xff, 0x00, 0xff,	/*  5:明るい紫 */
		0x00, 0xff, 0xff,	/*  6:明るい水色 */
		0xff, 0xff, 0xff,	/*  7:白 */
		0xc6, 0xc6, 0xc6,	/*  8:明るい灰色 */
		0x84, 0x00, 0x00,	/*  9:暗い赤 */
		0x00, 0x84, 0x00,	/* 10:暗い緑 */
		0x84, 0x84, 0x00,	/* 11:暗い黄色 */
		0x00, 0x00, 0x84,	/* 12:暗い青 */
		0x84, 0x00, 0x84,	/* 13:暗い紫 */
		0x00, 0x84, 0x84,	/* 14:暗い水色 */
		0x84, 0x84, 0x84	/* 15:暗い灰色 */
	};
	int i, r, g, b;
	if (binfo->vmode == 8) {
		for (i = 0; i < 16; i++) {
			table_8_565[i] = i;
		}
		set_palette(0, 15, table_rgb);
	} else {
		for (i = 0; i < 16; i++) {
			r = table_rgb[i * 3 + 0];
			g = table_rgb[i * 3 + 1];
			b = table_rgb[i * 3 + 2];
			table_8_565[i] = (unsigned short) (((r << 8) & 0xf800) |
							 ((g << 3) & 0x07e0) | (b >> 3));
		}
	}
	if (binfo->vmode == 8) {
		unsigned char table2[216 * 3];
		for (b = 0; b < 6; b++) {
			for (g = 0; g < 6; g++) {
				for (r = 0; r < 6; r++) {
					table2[(r + g * 6 + b * 36) * 3 + 0] = r * 51;
					table2[(r + g * 6 + b * 36) * 3 + 1] = g * 51;
					table2[(r + g * 6 + b * 36) * 3 + 2] = b * 51;
					table_8_565[r + g * 6 + b * 36 + 16] = r + g * 6 + b * 36 + 16;
				}
			}
		}
		set_palette(16, 231, table2);
	} else {
		for (b = 0; b < 6; b++) {
			for (g = 0; g < 6; g++) {
				for (r = 0; r < 6; r++) {
					table_8_565[r + g * 6 + b * 36 + 16] =
						(unsigned short) ((((r * 51) << 8) & 0xf800) |
						(((g * 51) << 3) & 0x07e0) | ((b * 51) >> 3));
				}
			}
		}
	}
	return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 割り込み許可フラグの値を記録する */
	io_cli(); 					/* 許可フラグを0にして割り込み禁止にする */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* 割り込み許可フラグを元に戻す */
	return;
}

void boxfill8(unsigned short *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = table_8_565[c];
	}
	return;
}

void init_screen8(short *vram, int x, int y)
{
	//	 ----+----1----+----2----+----3----+----4----
	static char logo[13][44] = {
		"..............................****....***.*.",
		".............................**OO**..**O**.O",
		"............................**OO.**O.**O.*O.",
		".**.*..****.******.***.**...**O..**O.**O..O.",
		"*****O**O**O.**O**O.**O**O.**OO..**O..**....",
		".**OO**OO**O.**O**O.**O**O.**O..**OO..***...",
		".**O.**O***O.**O**O**O**OO.**O..**O.*..**O..",
		"**OO.**O**OO**O**OO**O**O..**O.**OO**O.**O..",
		"**O..******.****OO.*******..****OO.*O***OO..",
		".OO...OOOOOO**OOO...OOOOOOO..OOOO...O.OOO...",
		"............**O.............................",
		"...........****.............................",
		"............OOOO............................"
	};
	int i, j, *fat;
	unsigned char c;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;

	boxfill8(vram, x, COL8_008484, 0, 0, x - 1, y - 29);

	fat = (int *) memman_alloc_4k(memman, 4 * 2880);
	file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
	read_picture(fat, vram, x, y);
	memman_free_4k(memman, (int) fat, 4 * 2880);

	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);

	for (j = 0; j < 13; j++) {
		for (i = 0; i < 44; i++) {
			c = logo[j][i];
			if (c == '*') {
				c = COL8_0000FF;
			} else if (c == 'O') {
				c = COL8_FFFFFF;
			} else {
				c = 0xff;
			}
			if (c != 0xff) {
				vram[(y - 18 + j) * x + (10 + i)] = table_8_565[c];
			}
		}
	}
	return;
}

void putfont8(short *vram, int xsize, int x, int y, unsigned char c, char scl, char *font)
{
	int i, j, k, l;
	char d;	/* data */
	short *p;
	unsigned char mask;
	for (i = 0; i < 16; i++) {
		d = font[i];
		mask = 0x80;
		for (j = 0; j < 8; j++) {
			p = vram + (y + i * scl) * xsize + x + j * scl;
			if ((d & mask) != 0) {
				for (k = 0; k < scl; k++) {
					for (l = 0; l < scl; l++) {
						p[k * xsize + l] = table_8_565[c];
					}
				}
			}
			mask >>= 1;
		}
	}
	return;
}

void putfonts8_asc(short *vram, int xsize, int x, int y, unsigned char c, char scl, unsigned char *s)
{
	extern char hankaku[4096];
	struct TASK *task = task_now();
	char *nihongo = (char *) *((int *) 0x0fe8), *font;
	int k, t;

	if (task->langmode == 0) {
		for (; *s != 0x00; s++) {
			putfont8(vram, xsize, x, y, c, scl, hankaku + *s * 16);
			x += scl * 8;
		}
	}
	if (task->langmode == 1) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if ((0x81 <= *s && *s <= 0x9f) || (0xe0 <= *s && *s <= 0xfc)) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, scl, nihongo + *s * 16);
				}
			} else {
				if (0x81 <= task->langbyte1 && task->langbyte1 <= 0x9f) {
					k = (task->langbyte1 - 0x81) * 2;
				} else {
					k = (task->langbyte1 - 0xe0) * 2 + 62;
				}
				if (0x40 <= *s && *s <= 0x7e) {
					t = *s - 0x40;
				} else if (0x80 <= *s && *s <= 0x9e) {
					t = *s - 0x80 + 63;
				} else {
					t = *s - 0x9f;
					k++;
				}
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, scl, font     );	/* 左半分 */
				putfont8(vram, xsize, x    , y, c, scl, font + 16);	/* 右半分 */
			}
			x += scl * 8;
		}
	}
	if (task->langmode == 2) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				if (0x81 <= *s && *s <= 0xfe) {
					task->langbyte1 = *s;
				} else {
					putfont8(vram, xsize, x, y, c, scl, nihongo + *s * 16);
				}
			} else {
				k = task->langbyte1 - 0xa1;
				t = *s - 0xa1;
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, scl, font     );	/* 左半分 */
				putfont8(vram, xsize, x    , y, c, scl, font + 16);	/* 右半分 */
			}
			x += scl * 8;
		}
	}
	return;
}

void init_mouse_cursor8(short *mouse, unsigned char bc)
/* マウスカーソルを準備（16x16） */
{
	static char cursor[16][16] = {
		"*...............",
		"**..............",
		"*O*.............",
		"*OO*............",
		"*O*O*...........",
		"*OO*O*..........",
		"*OOO*O*.........",
		"*OO*O*O*........",
		"*OOO*O*O*.......",
		"*OOOO*O*O*......",
		"*OOO*******.....",
		"*OO*OO**........",
		"*O**O****.......",
		"**.**...........",
		"*..*............",
		"................"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = table_8_565[COL8_000000];
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = table_8_565[COL8_FFFFFF];
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = table_8_565[bc];
			}
		}
	}
	return;
}

void putblock8_8(short *vram, int vxsize, int pxsize,
				 int pysize, int px0, int py0, short *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = table_8_565[buf[y * bxsize + x]];
		}
	}
	return;
}

void push_menu(short *vram, int bxsize, int bysize)
{
	int x, y;

	for (y = bysize - 5; y >= bysize - 23; y--) {
		for (x = 58; x >= 3; x--) {
			vram[(y + 1) * bxsize + (x + 1)] = vram[y * bxsize + x];
		}
	}

	boxfill8(vram, bxsize, COL8_FFFFFF,  2, bysize -  3, 59, bysize -  3);
	boxfill8(vram, bxsize, COL8_FFFFFF, 60, bysize - 24, 60, bysize -  3);
	boxfill8(vram, bxsize, COL8_848484,  3, bysize - 23, 59, bysize - 23);
	boxfill8(vram, bxsize, COL8_848484,  3, bysize - 22,  3, bysize -  5);
	boxfill8(vram, bxsize, COL8_000000,  2, bysize - 24, 59, bysize - 24);
	boxfill8(vram, bxsize, COL8_000000,  2, bysize - 23,  2, bysize -  4);
	vram[(bysize -  4) * bxsize +  3] = table_8_565[COL8_C6C6C6];
	vram[(bysize - 23) * bxsize + 59] = table_8_565[COL8_C6C6C6];

	return;
}

void pull_menu(short *vram, int bxsize, int bysize)
{
	int x, y;

	for (y = bysize - 22; y <= bysize - 4; y++) {
		for (x = 4; x <= 59; x++) {
			vram[(y - 1) * bxsize + (x - 1)] = vram[y * bxsize + x];
		}
	}

	boxfill8(vram, bxsize, COL8_FFFFFF,  2, bysize - 24, 59, bysize - 24);
	boxfill8(vram, bxsize, COL8_FFFFFF,  2, bysize - 23,  2, bysize -  4);
	boxfill8(vram, bxsize, COL8_848484,  3, bysize -  4, 59, bysize -  4);
	boxfill8(vram, bxsize, COL8_848484, 59, bysize - 22, 59, bysize -  5);
	boxfill8(vram, bxsize, COL8_000000,  2, bysize -  3, 59, bysize -  3);
	boxfill8(vram, bxsize, COL8_000000, 60, bysize - 24, 60, bysize -  3);

	return;
}

int read_picture(int *fat, short *vram, int x, int y)
{
	int i, j, x0, y0, fsize, info[4];
	unsigned char *filebuf, r, g, b;
	struct RGB *picbuf;
	struct MEMMAN *memman  = (struct MEMMAN *) MEMMAN_ADDR;
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	struct FILEINFO *finfo;
	struct DLL_STRPICENV *env;

	finfo = file_search("rapuwp.jpg", (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if (finfo == 0) {
		return -1;
	}
	fsize   = finfo->size;
	filebuf = (unsigned char *) memman_alloc_4k(memman, fsize);
	filebuf = file_loadfile2(finfo->clustno, &fsize, fat);

	env = (struct DLL_STRPICENV *) memman_alloc_4k(memman, sizeof(struct DLL_STRPICENV));
	info_JPEG(env, info, fsize, filebuf);
	picbuf  = (struct RGB *) memman_alloc_4k(memman, info[2] * info[3] * sizeof(struct RGB));
	decode0_JPEG(env, fsize, filebuf, 4, (unsigned char *) picbuf, 0);

	x0 = (int) ((x - info[2]) / 2);
	y0 = (int) ((y - info[3]) / 2);
	for (i = 0; i < info[3]; i++) {
		for (j = 0; j < info[2]; j++) {
			r = picbuf[i * info[2] + j].r;
			g = picbuf[i * info[2] + j].g;
			b = picbuf[i * info[2] + j].b;
			vram[(y0 + i) * x + (x0 + j)] = rgb2pal(r, g, b, j, i, binfo->vmode);
		}
	}

	memman_free_4k(memman, (int) filebuf, fsize);
	memman_free_4k(memman, (int) picbuf , info[2] * info[3] * sizeof(struct RGB));
	memman_free_4k(memman, (int) env    , sizeof(struct DLL_STRPICENV));

	return 0;
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
