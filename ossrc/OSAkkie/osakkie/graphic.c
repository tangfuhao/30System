/* �O���t�B�b�N�����֌W */

#include "bootpack.h"

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:�� */
		0xff, 0x00, 0x00,	/*  1:���邢�� */
		0x00, 0xff, 0x00,	/*  2:���邢�� */
		0xff, 0xff, 0x00,	/*  3:���邢���F */
		0x00, 0x00, 0xff,	/*  4:���邢�� */
		0xff, 0x00, 0xff,	/*  5:���邢�� */
		0x00, 0xff, 0xff,	/*  6:���邢���F */
		0xff, 0xff, 0xff,	/*  7:�� */
		0xc6, 0xc6, 0xc6,	/*  8:���邢�D�F */
		0x84, 0x00, 0x00,	/*  9:�Â��� */
		0x00, 0x84, 0x00,	/* 10:�Â��� */
		0x84, 0x84, 0x00,	/* 11:�Â����F */
		0x00, 0x00, 0x84,	/* 12:�Â��� */
		0x84, 0x00, 0x84,	/* 13:�Â��� */
		0x00, 0x84, 0x84,	/* 14:�Â����F */
		0x84, 0x84, 0x84,	/* 15:�Â��D�F */
	};
	static unsigned char table_rgb_osakkie[2 * 3] = {
		/* OSAkkie Only */
		0x3a, 0x6e, 0xa5,	/* 16:WinXP�Ȕw�i�F */
		0x00, 0x80, 0xff	/* 17:�^�C�g���o�[ */
	};
	unsigned char table_rgb2[216 * 3];
	int r, g, b, i;
	for (b = 0; b < 6; b++) {
		for (g = 0; g < 6; g++) {
			for (r = 0; r < 6; r++) {
				i = (r + g * 6 + b* 36) * 3;
				table_rgb2[i    ] = r * 51;
				table_rgb2[i + 1] = g * 51;
				table_rgb2[i + 2] = b * 51;
			}
		}
	}
	set_palette(0, 15, table_rgb);
	set_palette(16, 231, table_rgb2);
	set_palette(240, 241, table_rgb_osakkie);
	return;

	/* static char ���߂́A�f�[�^�ɂ����g���Ȃ�����DB���ߑ��� */
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* ���荞�݋��t���O�̒l���L�^���� */
	io_cli(); 					/* ���t���O��0�ɂ��Ċ��荞�݋֎~�ɂ��� */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* ���荞�݋��t���O�����ɖ߂� */
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

void picdata8(unsigned char *vram, int xsize, unsigned char *c, int sx, int sy, int trans, int px, int py)
{
	int x, y;

	for (y = py; y < (py+sy); y++) {
		for (x = px; x < (px+sx); x++) {
			if (trans < 0 || (0 <= trans && c[0] != trans))	// 0�ȏ�͓���
				vram[y * xsize + x] = c[0];
			c++;
		}
	}
	return;
}

void init_screen8(char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_3A6EA5, 0, 0, x - 1, y - 1);	// �f�X�N�g�b�v�w�i
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void putfonts8(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	struct TASK *task = task_now();
	char *nihongo = (char *) *((int *) 0x0fe8), *font;
	int k, t;

	if (task->langmode == 0) {
		for (; *s != 0x00; s++) {
			putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
			x += 8;
		}
	}
	if (task->langmode == 1) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				/* 1�o�C�g�� */
				if ((0x81 <= *s && *s <= 0x9f) || (0xe0 <= *s && *s <= 0xfc)) {
					/* 2�o�C�g�ڂ�����i�S�p�j */
					task->langbyte1 = *s;
				} else {
					 /* ���p���� */
					putfont8(vram, xsize, x, y, c, nihongo + *s * 16);
				}
			} else {
				/* 2�o�C�g�� */
				if (0x81 <= task->langbyte1 && task->langbyte1 <= 0x9f) {
					k = (task->langbyte1 - 0x81) * 2;
				} else {
					k = (task->langbyte1 - 0xe0) * 2 + 62;
				}
				if (0x40 <= *s && *s <= 0x7e) {
					t = *s - 0x40;		/* �������ق��̋�(0x40�`0x7e) */
				} else if (0x80 <= *s && *s <= 0x9e) {
					t = *s - 0x80 + 63;	/* �������ق��̋�(0x80�`0x9e) */
				} else {
					t = *s - 0x9f;		/* �傫���ق��̋�(0x9f�`0xfc) */
					k++;
				}
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, font     );	/* ���� */
				putfont8(vram, xsize, x    , y, c, font + 16);	/* �E�� */
			}
			x += 8;
		}
	}
	if (task->langmode == 2) {
		for (; *s != 0x00; s++) {
			if (task->langbyte1 == 0) {
				/* 1�o�C�g�� */
				if (0x81 <= *s && *s <= 0xfe) {
					task->langbyte1 = *s;	/* 2�o�C�g�ڂ�����i�S�p�j */
				} else {
					putfont8(vram, xsize, x, y, c, nihongo + *s * 16);	 /* ���p���� */
				}
			} else {
				/* 2�o�C�g�� */
				k = task->langbyte1 - 0xa1;
				t = *s - 0xa1;
				task->langbyte1 = 0;
				font = nihongo + 256 * 16 + (k * 94 + t) * 32;
				putfont8(vram, xsize, x - 8, y, c, font     );	/* ���� */
				putfont8(vram, xsize, x    , y, c, font + 16);	/* �E�� */
			}
			x += 8;
		}
	}
	return;
}

void init_mouse_cursor8(char *mouse, char bc)
/* �}�E�X�J�[�\���������i16x16�j */
{
	static char cursor[16][16] = {
		"*O.....O*O*O*O..",
		"O*O....O*****O..",
		".O*O..O*OOOOO*O.",
		"..O*OO*O**O**O*O",
		"...O***O*OO*OO*O",
		"....OO***O*OOO*O",
		"......O*O***O*O.",
		"......OO*O*O*O..",
		".....O*O*OOO*O*O",
		"....O*O**OOO**O*",
		".....O*O*OOO*O*O",
		"......O**OOO**O.",
		".....O*********O",
		".....O*O*O*O*O*O",
		"......O*O*O*O*O.",
		".......O*****O.."
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (cursor[y][x] == '*') {
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O') {
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}

void putminifont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 12; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x20) != 0) { p[0] = c; }
		if ((d & 0x10) != 0) { p[1] = c; }
		if ((d & 0x08) != 0) { p[2] = c; }
		if ((d & 0x04) != 0) { p[3] = c; }
		if ((d & 0x02) != 0) { p[4] = c; }
		if ((d & 0x01) != 0) { p[5] = c; }
	}
	return;
}

void putminifonts8(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char minifnt[4096];

	for (; *s != 0x00; s++) {
		putminifont8(vram, xsize, x, y, c, minifnt + *s * 12);
		x += 6;
	}
	return;
}
