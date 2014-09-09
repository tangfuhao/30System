/* ファイル関係 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

extern char t[];
extern struct CONSOLE *tcons;

void file_time(struct FILEINFO *finfo);
void file_compfat(int *fat, unsigned char *img, int clustno);

void file_readfat(int *fat, unsigned char *img)
/* ディスクイメージ内のFATの圧縮をとく */
{
	int i, j = 0;
	for (i = 0; i < 2880; i += 2) {
		fat[i + 0] = (img[j + 0]      | img[j + 1] << 8) & 0xfff;
		fat[i + 1] = (img[j + 1] >> 4 | img[j + 2] << 4) & 0xfff;
		j += 3;
	}
	return;
}

void file_loadfile(int clustno, int size, char *buf, int *fat, char *img)
{
	int i;
	for (;;) {
		if (size <= 512) {
			for (i = 0; i < size; i++) {
				buf[i] = img[clustno * 512 + i];
			}
			break;
		}
		for (i = 0; i < 512; i++) {
			buf[i] = img[clustno * 512 + i];
		}
		size -= 512;
		buf += 512;
		clustno = fat[clustno];
	}
	return;
}

struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max)
{
	int i, j;
	char s[12];
	for (j = 0; j < 11; j++) {
		s[j] = ' ';
	}
	j = 0;
	for (i = 0; j < 11 && name[i] != 0; i++) {
		if (name[i] == '.') {
			j = 8;
		} else {
			s[j] = name[i];
			if ('a' <= s[j] && s[j] <= 'z') {
				/* 小文字は大文字に直す */
				s[j] -= 0x20;
			} 
			j++;
		}
	}
	for (i = 0; i < max; ) {
		if (finfo[i].name[0] == 0x00) {
			break;
		}
		if ((finfo[i].type & 0x18) == 0) {
			for (j = 0; j < 11; j++) {
				if (finfo[i].name[j] != s[j]) {
					goto next;
				}
			}
			return finfo + i; /* ファイルが見つかった */
		}
next:
		i++;
	}
	return 0; /* 見つからなかった */
}

char *file_loadfile2(int clustno, int *psize, int *fat)
{
	int size = *psize, size2;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	char *buf, *buf2;
	buf = (char *) memman_alloc_4k(memman, size);
	file_loadfile(clustno, size, buf, fat, (char *) (ADR_DISKIMG + 0x003e00));
	if (size >= 17) {
		size2 = tek_getsize(buf);
		if (size2 > 0) {	/* tek圧縮がかかっていた */
			buf2 = (char *) memman_alloc_4k(memman, size2);
			tek_decomp(buf, buf2, size2);
			memman_free_4k(memman, (int) buf, size);
			buf = buf2;
			*psize = size2;
		}
	}
	return buf;
}

int file_searchfat(int *fat)
/* FATから空き領域を検索 */
{
	int i;
	for (i = 0; i < 2880; i++) {
		if (fat[i] == 0x0000) {
			return i;
		}
	}
	return 0x0fff;
}

int file_savefile(struct FILEHANDLE *fh, int size, int *fat, char *img)
{
	int nextno, clustno = fh->clustno, wsize = 0;
	char *buf = fh->buf;

	if (clustno >= 0x0ff8) {
		return 0;
	} else if (clustno != 0) {
		if (fat[clustno] == 0x0000 || fat[clustno] >= 0x0ff8) {
			fat[clustno] = 0x0fff;
			nextno = file_searchfat(fat);
			fat[clustno] = nextno;
			file_compfat(fat, img, clustno);
		} else {
			nextno = fat[clustno];
		}
	} else {
		nextno = fh->finfo->clustno;
	}
int i = 0;
char msg[30];
	for (; nextno != 0x0fff;) {
//sprintf(msg, "%04d ", clustno);
//cons_putstr0(tcons, msg);
		clustno = nextno;
sprintf(msg, "%04d ", clustno);
cons_putstr0(tcons, msg);
i++;
		if (size <= 512) {
			if (size != 0) {
				memcpy(img + clustno * 512, buf, size);
				wsize += size;
			}
			if (fat[clustno] == 0x0000) {
				fat[clustno] = 0x0fff;
				file_compfat(fat, img, clustno);
			}
			break;
		}
		memcpy(img + clustno * 512, buf, 512);
		size  -= 512;
		buf   += 512;
		wsize += 512;
		if (fat[clustno] == 0x0000 || fat[clustno] >= 0x0ff8) {
			fat[clustno] = 0x0fff;
			nextno = file_searchfat(fat);
			fat[clustno] = nextno;
			file_compfat(fat, img, clustno);
		} else {
			nextno = fat[clustno];
		}
	}
	fh->clustno = clustno;
	file_time(fh->finfo);
	fh->finfo->size += (wsize - (fh->finfo->size % 512));
cons_putstr0(tcons, "\n\0");
unsigned char comp[3];
for (i = 470; i < 488; i+=2) {
comp[0] = (unsigned char) *(img + 0x0200 + i / 2 * 3);
comp[1] = (unsigned char) *(img + 0x0200 + i / 2 * 3 + 1);
comp[2] = (unsigned char) *(img + 0x0200 + i / 2 * 3 + 2);
sprintf(msg, "%02x %02x %02x  ", comp[0], comp[1], comp[2]);
cons_putstr0(tcons, msg);
}

	return wsize;
}

int file_skipfile(struct FILEHANDLE *fh, int size, int *fat, char *img)
{
	int nextno = fh->finfo->clustno;
	int clustno = 0, ssize = 0, rsize;

	for (; ssize < size - 512;) {
		clustno = nextno;
		nextno  = fat[clustno];
		if (nextno >= 0x0ff8) {
			break;
		}
		ssize += 512;
	}
	rsize = size - ssize;
	if (rsize < 512) {
		memcpy(fh->buf, img + nextno * 512, rsize);
	} else if (rsize > 512) {
		rsize = 512;
		fh->finfo->size = ssize + rsize;
	}
	fh->pos     = ssize + rsize;
	fh->bpos    = rsize % 512;
	fh->clustno = clustno;

	return fh->pos;
}

struct FILEINFO *file_insert(char *name, struct FILEINFO *finfo, int max, int *fat)
{
	int i, j = 0, clustno;
	char s[13];

	for (i = 0; i < 12; i++) {
		if (i != 8) {
			s[i] = 0x20;
		}
	}
	for (i = 0; j < 12 && name[i] != 0; i++) {
		if (name[i] == '.') {
			j = 9;
		} else {
			s[j] = name[i];
			if ('a' <= s[j] && s[j] <= 'z') {
				s[j] -= 0x20;
			} 
			j++;
		}
	}

	for (i = 0; i < max; i++) {
		if (finfo[i].name[0] == 0x00 || finfo[i].name[0] == 0xe5) {
			clustno = file_searchfat(fat);
			if (clustno == 0x0fff) {
				break;
			}
			if (fat[clustno] == 0x0000) {
				fat[clustno] = 0x0fff;
			}
			strcpy(finfo[i].name, s);
			strcpy(finfo[i].ext, s + 9);
			finfo[i].type    = 0x20;
			finfo[i].clustno = clustno;
			finfo[i].size    = 0;
			file_time(&finfo[i]);
			return  finfo + i;
		}
	}

	return 0;
}

void file_time(struct FILEINFO *finfo)
{
	int i;
	char t16[7];
	unsigned short t10[7];

	memcpy(t16, t, 7);
	for (i = 0; i < 7; i++) {
		t10[i] = (t16[i] >> 4) * 10 + (t16[i] & 0x0f);
	}
	finfo->time = (t10[2] << 11)
				+ (t10[1] << 5)
				+ (unsigned short) (t10[0] / 2);
	finfo->date = ((t10[6] * 100 + t10[5] - 1980) << 9)
				+ (t10[4] << 5)
				+ t10[3];

	return;
}

void file_compfat(int *fat, unsigned char *img, int clustno)
{
	int clustlow, clustupp;
	unsigned char comp[3];

	if ((clustno % 2) == 0) {
		clustlow = clustno;
		clustupp = clustno + 1;
	} else {
		clustlow = clustno - 1;
		clustupp = clustno;
	}

	comp[0] = (unsigned char) (fat[clustlow] & 0x0ff);
	comp[1] = (unsigned char) ((fat[clustupp] & 0x00f) << 4 | (fat[clustlow] & 0xf00) >> 8);
	comp[2] = (unsigned char) ((fat[clustupp] & 0xff0) >> 4);

	memcpy(img + 0x0200 + clustlow / 2 * 3, comp, 3);
	memcpy(img + 0x1400 + clustlow / 2 * 3, comp, 3);

	return;
}
