#include "../bayos.h"
//#include "../sys/font_latin1.h"
#include "../sys/font_hankaku.h"

struct SHTCTL *SheetCtl::ctl  = NULL;
struct Sheet  *SheetCtl::back = NULL;

/* ������ */
void SheetCtl::init()
{
	/* ��ʂ̃p�����[�^ */
	BOOT_PARAM *boot_param = (BOOT_PARAM *) ADR_BOOTINFO;
	
	/* ���~���ꗗ�̏����� */
	ctl = (struct SHTCTL *) malloc(sizeof (struct SHTCTL));
	if (ctl == 0) {
		return;
	}
	ctl->vram  = (word *) boot_param->vram;
	ctl->xsize = boot_param->screen_x;
	ctl->ysize = boot_param->screen_y;
	ctl->map   = (unsigned char *) malloc(ctl->xsize * ctl->ysize);
	if (ctl->map == 0) {
		free(ctl);
		return;
	}
	ctl->top = -1; /* �V�[�g�͈ꖇ���Ȃ� */
	for (int i = 0; i < MAX_SHEETS; i++) {
		ctl->sheets0[i].flags = 0; /* ���g�p�}�[�N */
	}
	
	/* �ǎ� */
	back = SheetCtl::allocate();
	word* buffer = (word *) malloc(ctl->xsize * ctl->ysize * sizeof(short));
	SheetCtl::setbuf(back, buffer, ctl->xsize, ctl->ysize, -1);
	FileEntry* entry = File::load("BAYOS.BM5");
	if (entry != NULL) {
		byte* aData = entry->data;
		#if 0
		/* �ǎ����Q�{�Ɉ����L�΂� */
		for (int i = 0; i < WALLPAPER_HEIGHT; i++) {
			for (int j = 0; j < WALLPAPER_WIDTH; j++) {
				int offset = 0x36 + i * WALLPAPER_WIDTH * 3 + j * 3;
				byte b = aData[offset];
				byte g = aData[offset + 1];
				byte r = aData[offset + 2];
				//byte color = Screen::change888to8(r << 16 | g << 8 | b);
				for (int k = 0; k < WALLPAPER_SCALE; k++) {
					for (int l = 0; l < WALLPAPER_SCALE; l++) {
						int x = j * WALLPAPER_SCALE + l;
						int y = 479 - (i * WALLPAPER_SCALE + k);
						back->buffer[y * ctl->xsize + x] = color;
					}
				}
			}
		}
		#endif
		#if 1
		/* �ǎ����Q�����E�Ώ̂ɓ\�� */
		for (int i = 0; i < WALLPAPER_HEIGHT; i++) {
			for (int j = 0; j < WALLPAPER_WIDTH; j++) {
				int offset = 0x36 + i * WALLPAPER_WIDTH * 3 + j * 3;
				byte b = aData[offset];
				byte g = aData[offset + 1];
				byte r = aData[offset + 2];
				//byte color = Screen::change888to8(r << 16 | g << 8 | b);
			    word color = (word)(((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | (b >> 3));
			    back->buffer[(ctl->ysize - 1 - i) * ctl->xsize + j] = color;
			    back->buffer[(WALLPAPER_HEIGHT - i - 1) * ctl->xsize + j] = color;
			}
		}
		#endif
		//delete(entry);
	}
	SheetCtl::updown(back, 0);
}

/* ���~�����擾���� */
struct Sheet *SheetCtl::allocate()
{
	struct Sheet *sht;
	for (int i = 0; i < MAX_SHEETS; i++) {
		if (ctl->sheets0[i].flags == 0) {
			sht = &ctl->sheets0[i];
			sht->flags =  1; /* �g�p���}�[�N */
			sht->depth = -1; /* ��\���� */
			sht->task  =  0; /* �����ŕ���@�\���g��Ȃ� */
			return sht;
		}
	}
	return 0;	/* �S�ẴV�[�g���g�p�������� */
}

/* ���~���̃T�C�Y�ƃo�b�t�@��ݒ肷�� */
void SheetCtl::setbuf(struct Sheet *sht, unsigned short *buf, int xsize, int ysize, int transparent)
{
	sht->buffer      = buf;
	sht->width       = xsize;
	sht->height      = ysize;
	sht->transparent = transparent;
}

/* �}�b�v�X�V�i�����֐��j*/
void SheetCtl::refreshmap(int vx0, int vy0, int vx1, int vy1, int h0)
{
	int h, bx, by, vx, vy, bx0, by0, bx1, by1/*, sid4, *p */;
	unsigned char sid, *map = ctl->map;
    unsigned short *buf;
	struct Sheet *sht;
	if (vx0 < 0) { vx0 = 0; }
	if (vy0 < 0) { vy0 = 0; }
	if (vx1 > ctl->xsize) { vx1 = ctl->xsize; }
	if (vy1 > ctl->ysize) { vy1 = ctl->ysize; }
	for (h = h0; h <= ctl->top; h++) {
		sht = ctl->sheets[h];
		sid = sht - ctl->sheets0; /* �Ԓn�������Z���Ă�����������ԍ��Ƃ��ė��p */
		buf = sht->buffer;
		bx0 = vx0 - sht->vx0;
		by0 = vy0 - sht->vy0;
		bx1 = vx1 - sht->vx0;
		by1 = vy1 - sht->vy0;
		if (bx0 < 0) { bx0 = 0; }
		if (by0 < 0) { by0 = 0; }
		if (bx1 > sht->width) { bx1 = sht->width; }
		if (by1 > sht->height) { by1 = sht->height; }
		if (sht->transparent == -1) {
		    #if 0
			if ((sht->vx0 & 3) == 0 && (bx0 & 3) == 0 && (bx1 & 3) == 0) {
				/* �����F�Ȃ���p�̍����Łi4�o�C�g�^�j */
				bx1 = (bx1 - bx0) / 4; /* MOV�� */
				sid4 = sid | sid << 8 | sid << 16 | sid << 24;
				for (by = by0; by < by1; by++) {
					vy = sht->vy0 + by;
					vx = sht->vx0 + bx0;
					p = (int *) &map[vy * ctl->xsize + vx];
					for (bx = 0; bx < bx1; bx++) {
						p[bx] = sid4;
					}
				}
			} else {
			#endif
				/* �����F�Ȃ���p�̒ᑬ�Łi1�o�C�g�^�j */
				for (by = by0; by < by1; by++) {
					vy = sht->vy0 + by;
					for (bx = bx0; bx < bx1; bx++) {
						vx = sht->vx0 + bx;
						map[vy * ctl->xsize + vx] = sid;
					}
				}
			//}
		} else {
			/* �����F����̈�ʔ� */
			for (by = by0; by < by1; by++) {
				vy = sht->vy0 + by;
				for (bx = bx0; bx < bx1; bx++) {
					vx = sht->vx0 + bx;
					if (buf[by * sht->width + bx] != sht->transparent) {
						map[vy * ctl->xsize + vx] = sid;
					}
				}
			}
		}
	}
}

/* ��ʍX�V�i�����֐��j*/
void SheetCtl::refreshsub(int vx0, int vy0, int vx1, int vy1, int h0, int h1)
{
	int h, bx, by, vx, vy, bx0, by0, bx1, by1/*, bx2, sid4, i, i1, *p, *q, *r */;
	unsigned char *map = ctl->map, sid;
    unsigned short *vram = ctl->vram, *buf;
	struct Sheet *sht;
	/* refresh�͈͂���ʊO�ɂ͂ݏo���Ă�����␳ */
	if (vx0 < 0) { vx0 = 0; }
	if (vy0 < 0) { vy0 = 0; }
	if (vx1 > ctl->xsize) { vx1 = ctl->xsize; }
	if (vy1 > ctl->ysize) { vy1 = ctl->ysize; }
	for (h = h0; h <= h1; h++) {
		sht = ctl->sheets[h];
		buf = sht->buffer;
		sid = sht - ctl->sheets0;
		/* vx0�`vy1���g���āAbx0�`by1���t�Z���� */
		bx0 = vx0 - sht->vx0;
		by0 = vy0 - sht->vy0;
		bx1 = vx1 - sht->vx0;
		by1 = vy1 - sht->vy0;
		if (bx0 < 0) { bx0 = 0; }
		if (by0 < 0) { by0 = 0; }
		if (bx1 > sht->width) { bx1 = sht->width; }
		if (by1 > sht->height) { by1 = sht->height; }
	    #if 0
		if ((sht->vx0 & 3) == 0) {
			/* 4�o�C�g�^ */
			i  = (bx0 + 3) / 4; /* bx0��4�Ŋ��������́i�[���؂�グ�j */
			i1 =  bx1      / 4; /* bx1��4�Ŋ��������́i�[���؂�̂āj */
			i1 = i1 - i;
			sid4 = sid | sid << 8 | sid << 16 | sid << 24;
			for (by = by0; by < by1; by++) {
				vy = sht->vy0 + by;
				for (bx = bx0; bx < bx1 && (bx & 3) != 0; bx++) {	/* �O�̒[����1�o�C�g���� */
					vx = sht->vx0 + bx;
					if (map[vy * ctl->xsize + vx] == sid) {
						vram[vy * ctl->xsize + vx] = buf[by * sht->width + bx];
					}
				}
				vx = sht->vx0 + bx;
				p = (int *) &map[vy * ctl->xsize + vx];
				q = (int *) &vram[vy * ctl->xsize + vx];
				r = (int *) &buf[by * sht->width + bx];
				for (i = 0; i < i1; i++) {							/* 4�̔{������ */
					if (p[i] == sid4) {
						q[i] = r[i];
					} else {
						bx2 = bx + i * 4;
						vx = sht->vx0 + bx2;
						if (map[vy * ctl->xsize + vx + 0] == sid) {
							vram[vy * ctl->xsize + vx + 0] = buf[by * sht->width + bx2 + 0];
						}
						if (map[vy * ctl->xsize + vx + 1] == sid) {
							vram[vy * ctl->xsize + vx + 1] = buf[by * sht->width + bx2 + 1];
						}
						if (map[vy * ctl->xsize + vx + 2] == sid) {
							vram[vy * ctl->xsize + vx + 2] = buf[by * sht->width + bx2 + 2];
						}
						if (map[vy * ctl->xsize + vx + 3] == sid) {
							vram[vy * ctl->xsize + vx + 3] = buf[by * sht->width + bx2 + 3];
						}
					}
				}
				for (bx += i1 * 4; bx < bx1; bx++) {				/* ���̒[����1�o�C�g���� */
					vx = sht->vx0 + bx;
					if (map[vy * ctl->xsize + vx] == sid) {
						vram[vy * ctl->xsize + vx] = buf[by * sht->width + bx];
					}
				}
			}
		} else {
		#endif
			/* 1�o�C�g�^ */
			for (by = by0; by < by1; by++) {
				vy = sht->vy0 + by;
				for (bx = bx0; bx < bx1; bx++) {
					vx = sht->vx0 + bx;
					if (map[vy * ctl->xsize + vx] == sid) {
						vram[vy * ctl->xsize + vx] = buf[by * sht->width + bx];
					}
				}
			}
		//}
	}
}

/* ���~���̐[���X�V */
void SheetCtl::updown(struct Sheet *sht, int depth)
{
	int h, old = sht->depth; /* �ݒ�O�̍������L������ */

	/* �w�肪�Ⴗ���⍂������������A�C������ */
	if (depth > ctl->top + 1) {
		depth = ctl->top + 1;
	}
	if (depth < -1) {
		depth = -1;
	}
	sht->depth = depth; /* ������ݒ� */

	/* �ȉ��͎��sheets[]�̕��בւ� */
	if (old > depth) {	/* �ȑO�����Ⴍ�Ȃ� */
		if (depth >= 0) {
			/* �Ԃ̂��̂������グ�� */
			for (h = old; h > depth; h--) {
				ctl->sheets[h] = ctl->sheets[h - 1];
				ctl->sheets[h]->depth = h;
			}
			ctl->sheets[depth] = sht;
			SheetCtl::refreshmap(sht->vx0, sht->vy0, sht->vx0 + sht->width, sht->vy0 + sht->height, depth + 1);
			SheetCtl::refreshsub(sht->vx0, sht->vy0, sht->vx0 + sht->width, sht->vy0 + sht->height, depth + 1, old);
		} else {	/* ��\���� */
			if (ctl->top > old) {
				/* ��ɂȂ��Ă�����̂����낷 */
				for (h = old; h < ctl->top; h++) {
					ctl->sheets[h] = ctl->sheets[h + 1];
					ctl->sheets[h]->depth = h;
				}
			}
			ctl->top--; /* �\�����̉������������̂ŁA��ԏ�̍��������� */
			SheetCtl::refreshmap(sht->vx0, sht->vy0, sht->vx0 + sht->width, sht->vy0 + sht->height, 0);
			SheetCtl::refreshsub(sht->vx0, sht->vy0, sht->vx0 + sht->width, sht->vy0 + sht->height, 0, old - 1);
		}
	} else if (old < depth) {	/* �ȑO���������Ȃ� */
		if (old >= 0) {
			/* �Ԃ̂��̂����������� */
			for (h = old; h < depth; h++) {
				ctl->sheets[h] = ctl->sheets[h + 1];
				ctl->sheets[h]->depth = h;
			}
			ctl->sheets[depth] = sht;
		} else {	/* ��\����Ԃ���\����Ԃ� */
			/* ��ɂȂ���̂������グ�� */
			for (h = ctl->top; h >= depth; h--) {
				ctl->sheets[h + 1] = ctl->sheets[h];
				ctl->sheets[h + 1]->depth = h + 1;
			}
			ctl->sheets[depth] = sht;
			ctl->top++; /* �\�����̉��������������̂ŁA��ԏ�̍����������� */
		}
		SheetCtl::refreshmap(sht->vx0, sht->vy0, sht->vx0 + sht->width, sht->vy0 + sht->height, depth);
		SheetCtl::refreshsub(sht->vx0, sht->vy0, sht->vx0 + sht->width, sht->vy0 + sht->height, depth, depth);
	}
}

/* ���~���̕����ĕ`�� */
void SheetCtl::refresh(struct Sheet *sht, int bx0, int by0, int bx1, int by1)
{
	if (sht->depth >= 0) { /* �������\�����Ȃ�A�V�����������̏��ɉ����ĉ�ʂ�`������ */
		SheetCtl::refreshsub(sht->vx0 + bx0, sht->vy0 + by0, sht->vx0 + bx1, sht->vy0 + by1, sht->depth, sht->depth);
	}
}

/* ���~���̈ړ� */
void SheetCtl::slide(struct Sheet *sht, int vx0, int vy0)
{
	int old_vx0 = sht->vx0, old_vy0 = sht->vy0;
	vx0 = vx0 & ~1; /* 2�h�b�g�������������Ȃ� */
	sht->vx0 = vx0;
	sht->vy0 = vy0;
	if (sht->depth >= 0) { /* �������\�����Ȃ�A�V�����������̏��ɉ����ĉ�ʂ�`������ */
		SheetCtl::refreshmap(old_vx0, old_vy0, old_vx0 + sht->width, old_vy0 + sht->height, 0);
		SheetCtl::refreshmap(vx0, vy0, vx0 + sht->width, vy0 + sht->height, sht->depth);
		SheetCtl::refreshsub(old_vx0, old_vy0, old_vx0 + sht->width, old_vy0 + sht->height, 0, sht->depth - 1);
		SheetCtl::refreshsub(vx0, vy0, vx0 + sht->width, vy0 + sht->height, sht->depth, sht->depth);
	}
}

/* ���~���̔j�� */
void SheetCtl::dispose(struct Sheet *sht)
{
	if (sht->depth >= 0) {
		SheetCtl::updown(sht, -1); /* �\�����Ȃ�܂���\���ɂ��� */
	}
	sht->flags = 0; /* ���g�p�}�[�N */
}

/* �_��ł� */
void SheetCtl::drawPixel(struct Sheet *sht, int x, int y, word col)
{
	int tx = 0;
	int ty = 0;
	int cw = sht->width;
	sht->buffer[(tx + x) + cw * (ty + y)] = col;
}

/* �������� */
void SheetCtl::drawLine(struct Sheet *sht, int x0, int y0, int x1, int y1, word col)
{
	int tx = 0;
	int ty = 0;
	int cw = sht->width;
	int dx = (x0 > x1) ? (x0 - x1) : (x1 - x0);
	int dy = (y0 > y1) ? (y0 - y1) : (y1 - y0);
	
	if (dx == 0 && dy == 0) {
		sht->buffer[(tx + x0) + cw * (ty + y0)] = col;
	} else if (dx >= dy) {
		if (x0 > x1) {
			int tx = x0; x0 = x1; x1 = tx;
			int ty = y0; y0 = y1; y1 = ty;
		}
		for (int x = x0; x <= x1; x++) {
			sht->buffer[(tx + x) + cw * (ty + ((2 * y0 + 2 * (y1 - y0) * (x - x0) / (x1 - x0)) + 1) / 2)] = col;
		}
	} else {
		if (y0 > y1) {
			int tx = x0; x0 = x1; x1 = tx;
			int ty = y0; y0 = y1; y1 = ty;
		}
		for (int y = y0; y <= y1; y++) {
			sht->buffer[(tx + (2 * x0 + 2 * (x1 - x0) * (y - y0) / (y1 - y0) + 1) / 2) + cw * (ty + y)] = col;
		}
	}
}

/* ��`��`�� */
void SheetCtl::drawRect(struct Sheet *sht, int x, int y, int width, int height, word col)
{
	int tx = 0;
	int ty = 0;
	int cw = sht->width;
	int xw = x + width  - 1;
	int yh = y + height - 1;
	for (int xx = x; xx <= xw; xx++) {
		sht->buffer[(tx + xx) + cw * (ty + y)]  = col;
		sht->buffer[(tx + xx) + cw * (ty + yh)] = col;
	}
	for (int yy = y + 1; yy <= yh - 1; yy++) {
		sht->buffer[(tx + x)  + cw * (ty + yy)] = col;
		sht->buffer[(tx + xw) + cw * (ty + yy)] = col;
	}
}

/* ��`��h��Ԃ� */
void SheetCtl::fillRect(struct Sheet *sht, int x0, int y0, int x1, int y1, word col)
{
	int tx = 0;
	int ty = 0;
	int cw = sht->width;
	for (int y = y0; y < y0 + y1; y++) {
		for (int x = x0; x < x0 + x1; x++) {
			sht->buffer[(tx + x) + cw * (ty + y)] = col;
		}
	}
}

/* �������`�悷�� */
void SheetCtl::drawString(struct Sheet *sht, char* title, int x0, int y0)
{
	int w = sht->width;
	int I = strlen(title);
	
	for (int i = 0; i < I; i++) {
		int  pos = 0;
		byte chr = title[i] & 0xff;
		
		/* ���p�p���� */
		if (0x20 <= chr && chr < 0x7f) {
			pos = chr - 0x20;
		}
		/* ���p�J�^�J�i */
		else if (0xA1 <= chr && chr <= 0xdf) {
			pos = chr - 0xa1 + 95;
		}
		/* ����ȊO */
		else {
			continue;
		}
		
		int cnt = 2;
		int bit = 1;
		
		for (int y = 0; y < FONT_H; y++) {
			for (int x = 0; x < FONT_W; x++) {
				if ((font_hankaku[pos][cnt] & bit) != 0) {
					sht->buffer[(x0 + x + i * FONT_W) + w * (y0 + y)] = BLACK;
				}
				bit <<= 1;
				if (bit == 256) {
					cnt++;
					bit = 1;
				}
			}
		}
	}
}

/* �g��`�� */
void SheetCtl::drawFrame(struct Sheet *sht, char* title)
{
	int w = sht->width;
	int h = sht->height;
	
	/* �w�i�h��Ԃ� */
	//memset(sht->buffer, 0x8, w * h);
    fillRect(sht, 0, 0, w, h, LIGHT_GRAY);
	
	/* ����{�^�� */
	static byte icon_close_data [] = {
		0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x2,
		0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,
		0x1,0x0,0x8,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x0,0x8,
		0x1,0x0,0x2,0x5,0x5,0x3,0x3,0x7,0x7,0x2,0x1,0x0,0x8,
		0x1,0x0,0x2,0x5,0x3,0x3,0x7,0x7,0x2,0x2,0x1,0x0,0x8,
		0x1,0x0,0x2,0x3,0x3,0x7,0x7,0x2,0x2,0x6,0x1,0x0,0x8,
		0x1,0x0,0x2,0x3,0x7,0x7,0x2,0x2,0x6,0x6,0x1,0x0,0x8,
		0x1,0x0,0x2,0x7,0x7,0x2,0x2,0x6,0x6,0x4,0x1,0x0,0x8,
		0x1,0x0,0x2,0x7,0x2,0x2,0x6,0x6,0x4,0x4,0x1,0x0,0x8,
		0x1,0x0,0x2,0x2,0x2,0x6,0x6,0x4,0x4,0x8,0x1,0x0,0x8,
		0x1,0x0,0x2,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x8,
		0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,
		0x2,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,
	};
	static word icon_close_pallete [] = {
		0x18c3,
		0x8c71,
		0xce79,
		0xad75,
		0xef7d,
		0x9cf3,
		0xdefb,
		0xbdf7,
		0xffff,
	};
	
	for (int y = 0; y < 13; y++) {
		for (int x = 0; x < 13; x++) {
			sht->buffer[(y + 4) * w + (x + 4)] = 
				icon_close_pallete[icon_close_data[y * 13 + x]];
		}
	}
	
	/* �O�g */
	drawRect(sht, 0, 0, w, h, BLACK);
	
	/* ���g */
	drawRect(sht, 5, 21, w - 10, h - 26, BLACK);
	
	/* ���� */
	drawLine(sht, 4, 20, 4, h - 6,  DARK_GRAY);
	drawLine(sht, 4, 20, w - 6, 20, DARK_GRAY);
	drawLine(sht, w - 5, 21, w - 5, h - 6, WHITE);
	drawLine(sht, 4, h - 5,  w - 5, h - 5, WHITE);
	drawLine(sht, 1, 1, 1, h - 3, WHITE);
	drawLine(sht, 1, 1, w - 3, 1, WHITE);
	drawLine(sht, w - 2, 2, w - 2, h - 2, DARK_GRAY);
	drawLine(sht, 2, h - 2, w - 3, h - 2, DARK_GRAY);
	
	/* �^�C�g���� */
	int L = strlen(title) * 6 + FONT_W;
	int W = (w - L) / 2;
	
	/* �^�C�g���� */
	for (int y = 4; y <= 14; y += 2) {
		drawLine(sht, 21, y, W - 1, y, WHITE);
		drawLine(sht, 22, y + 1, W, y + 1, DARK_GRAY);
		drawLine(sht, W + L, y, w - 6, y, WHITE);
		drawLine(sht, W + L + 1, y + 1, w - 5, y + 1, DARK_GRAY);
	}

	/* �^�C�g�� */
	drawString(sht, title, W + FONT_W / 2, ((22 - FONT_H) / 2));
	drawString(sht, title, W + FONT_W / 2 + 1, ((22 - FONT_H) / 2));
}
