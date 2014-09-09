//===================================================================
//
//	Puzzle ScreenSaver for rapuOS
//
//	puzzle.c 2006 rapper
//
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apilib.h"

#define BMP_MAX		2
#define FRAME_DEF	100
#define FRAME_MAX	32767
#define min(a, b)	(((a) < (b)) ? (a) : (b))

struct POINT {
	int x, y;
};
struct SYSINFO {
	int scr_x;
	int scr_y;
	int scr_c;
};
struct DLL_STRPICENV {	/* 64KB */
	int work[64 * 1024 / 4];
};
struct RGB {
	unsigned char b, g, r, t;
};

/* 0～32767の範囲で乱数を発生 */
int  rand(void);

/* bmp.nasm */
int  info_BMP(struct DLL_STRPICENV *env, int *info, int size, char *fp);
int  decode0_BMP(struct DLL_STRPICENV *env, int size, char *fp, int b_type, char *buf, int skip);

/* jpeg.c */
int  info_JPEG(struct DLL_STRPICENV *env, int *info, int size, char *fp);
int  decode0_JPEG(struct DLL_STRPICENV *env, int size, char *fp, int b_type, char *buf, int skip);

int  InitWindow(void);
void MainLoop(void);
void Terminate(void);
void InitDimension(void);
void NextCut(void);
int  GetFileInfo(void);
int  GetFrameNum(void);
void DisplayClock(void);
int  ReadBitmap(int *info, int fsize);
int  LoadImage(void);
unsigned short rgb2pal(int r, int g, int b, int x, int y, int cb);

int			V_WIDTH, V_HEIGHT, V_COLBIT;
int			pwidth, pheight;
int			bwidth, bheight;
int			ltpointx, ltpointy;
int			row, col;
int			nrow, ncol;
int			srow, scol;
int			brow, bcol;
int			frame;
int			fnumber;
int			matrix[10][13];
char		*mvlst;
char		t[7], osec;

int			scene;
int			display;
struct		POINT loc[4];

int			bmpnum;
int			cutcnt;

struct		BMPINFO
{
	int		type;
	int		size;
	int		brow;
	int		bcol;
	int		bwidth;
	int		bheight;
	int		pwidth;
	int		pheight;
	int		owidth;
	char	filepath[12];
} bmpinfo[BMP_MAX];

int			win, timer;
int			term = 0;
char		*filebuf;
short		*wbuf, *pbuf;
struct		DLL_STRPICENV env;

//===================================================================
//
//	HariMain
//
//　メイン関数
//
//===================================================================
void HariMain(void)
{
	int			key;
	char		t[7];
	unsigned	int	seed;

	api_systime(t);
	seed = t[5] * 32140800 + t[4] * 2678400 + t[3] * 86400
		 + t[2] * 3600     + t[1] * 60      + t[0];
	srand(seed);

	if (! InitWindow()) {
		api_end();
	}

	InitDimension();
	frame = fnumber;

	for (;;) {
		frame--;
		switch (frame) {
		case -1:
			api_bitbltEx(win, pbuf, ltpointx, ltpointy, 0, 0, pwidth, pheight, pwidth);
			break;
		case -100:
			if (cutcnt >= bmpnum - 1) {
				cutcnt = 0;
			} else {
				cutcnt++;
			}
			NextCut();
			InitDimension();
			frame = fnumber;
			break;
		default:
			if (frame >= 0) {
				MainLoop();
			} else {
				if (display) {
					DisplayClock();
					api_refreshwin(win, 0, 0, V_WIDTH, V_HEIGHT);
				}
			}
			break;
		}
		if (term) { break; }

		api_settimer(timer, 5);
		for (;;) {
			key = api_getkey(1);
			if (key == 128 || key == 0x0a) { break; }
		}
		if (key == 0x0a) { break; }
	}

	Terminate();
}

//===================================================================
//
//	InitWindow
//
//	ウィンドウを生成する
//
//===================================================================
int InitWindow(void)
{
	struct SYSINFO sysinfo;

	api_sysinfo(&sysinfo);
	V_WIDTH  = sysinfo.scr_x;
	V_HEIGHT = sysinfo.scr_y;
	V_COLBIT = sysinfo.scr_c;

	api_initmalloc();
	filebuf = api_malloc(512 * 1024);
	if (! (bmpnum = GetFileInfo())) {
		return(0);
	}

	brow    = bmpinfo[0].brow;
	bcol    = bmpinfo[0].bcol;
	bwidth  = bmpinfo[0].bwidth;
	bheight = bmpinfo[0].bheight;
	pwidth  = bmpinfo[0].pwidth;
	pheight = bmpinfo[0].pheight;

	LoadImage();

	if (! GetFrameNum()) {
		return(0);
	}

	wbuf = api_malloc(V_WIDTH * V_HEIGHT * 2);
	win  = api_openwin(wbuf, V_WIDTH, V_HEIGHT, -1, "");
	api_autorefresh(win, 0);

	timer = api_alloctimer();
	api_inittimer(timer, 128);

	ltpointx = (V_WIDTH  - pwidth ) / 2;
	ltpointy = (V_HEIGHT - pheight) / 2;
	if (pwidth <= V_WIDTH - 608 || pheight <= V_HEIGHT - 64) {
		display = 1;
	} else {
		display = 0;
	}

	loc[0].x = V_WIDTH  - 305;
	loc[0].y = V_HEIGHT -  33;
	loc[1].x = 0;
	loc[1].y = 0;
	loc[2].x = loc[1].x;
	loc[2].y = loc[0].y;
	loc[3].x = loc[0].x;
	loc[3].y = loc[1].y;
	scene    = 3;
	cutcnt   = 0;

	return(1);
}

//===================================================================
//
//	MainLoop
//
//	メインループ
//
//===================================================================
void MainLoop(void)
{
	int		i;
	int		xmov = 0, ymov = 0;
	int		endc = 0, key;
	int		dleft, dtop;
	int		cleft, ctop;
	int		cwidth, cheight;

	switch (mvlst[frame]) {
	case 1:
		xmov = -1;
		ymov =  0;
		nrow =  row;
		ncol =  col + 1;
		endc =  bwidth;
		break;
	case 2:
		xmov =  0;
		ymov =  1;
		nrow =  row - 1;
		ncol =  col;
		endc =  bheight;
		break;
	case 3:
		xmov =  1;
		ymov =  0;
		nrow =  row;
		ncol =  col - 1;
		endc =  bwidth;
		break;
	case 4:
		xmov =  0;
		ymov = -1;
		nrow =  row + 1;
		ncol =  col;
		endc =  bheight;
		break;
	}

	dtop    = nrow * bheight;
	dleft   = ncol * bwidth;
	srow    = matrix[nrow][ncol] / bcol;
	scol    = matrix[nrow][ncol] % bcol;

	ctop    = min(dtop, dtop+ymov * bheight);
	cleft   = min(dleft, dleft + xmov * bwidth);
	cwidth  = bwidth  * (1 + abs(xmov));
	cheight = bheight * (1 + abs(ymov));

	for (i = 0; i < endc; i++) {
		if ((i % 2) == 0) {
			api_settimer(timer, 1);
			for (;;) {
				key = api_getkey(1);
				if (key ==  128) { break;    }
				if (key == 0x0a) { term = 1; }
			}
		}

		api_boxfilwin(win, ltpointx + dleft, ltpointy + dtop,
			ltpointx + dleft + bwidth - 1, ltpointy + dtop + bheight - 1, 0);
		dleft += xmov;
		dtop  += ymov;
		api_bitbltEx(win, pbuf, ltpointx + dleft, ltpointy + dtop,
			scol * bwidth, srow * bheight, bwidth, bheight, pwidth);

		if (display) {
			DisplayClock();
		}
		api_refreshwin(win, 0, 0, V_WIDTH, V_HEIGHT);
	}

	matrix[row][col]   = matrix[nrow][ncol];
	matrix[nrow][ncol] = brow * bcol - 1;
	row                = nrow;
	col                = ncol;

	return;
}

//===================================================================
//
//	Terminate
//
//	終了処理
//
//===================================================================
void Terminate()
{
	api_free(wbuf, V_WIDTH * V_HEIGHT * 2);
	api_free(mvlst, fnumber);
	api_free(pbuf, pwidth * pheight * 2);
	api_free(filebuf, 512 * 1024);
	api_closewin(win);
	api_end();

	return;
}

//===================================================================
//
//	InitDimension
//
//	パズルを分散する
//
//===================================================================
void InitDimension(void)
{
	int		i, j, k;
	int		step = 1, key;
	int		xmov = 0, ymov = 0;
	int		xbgn = 0, ybgn = 0;
	int		fwidth, fheight;
	int		fpointx, fpointy;
	int		minb, endc, stat;
	char	u, flg;

	api_boxfilwin(win, 0, 0, V_WIDTH - 1, V_HEIGHT - 1, 0);

	for (i = 0; i < brow; i++) {
		for (j = 0; j < bcol; j++) {
			matrix[i][j] = i * bcol + j;
		}
	}

	row = brow - 1;
	col = bcol - 1;

	for (k = 0; k < fnumber; k++) {
		u   = rand() % 4 + 1;
		flg = 0;

		while (! flg) {
			switch(u) {
			case 1:
				if (col >= 1 && (k == 0 || mvlst[k - 1] != 3)) {
					nrow = row;
					ncol = col - 1;
					flg  = 1;
				}
				break;
			case 2:
				if (row <= brow - 2 && (k == 0 || mvlst[k - 1] != 4)) {
					nrow = row + 1;
					ncol = col;
					flg  = 1;
				}
                break;
			case 3:
				if (col <= bcol - 2 && (k == 0 || mvlst[k - 1] != 1)) {
					nrow = row;
					ncol = col + 1;
					flg  = 1;
				}
				break;
			case 4:
				if (row >= 1 && (k == 0 || mvlst[k - 1] != 2)) {
					nrow = row - 1;
					ncol = col;
					flg  = 1;
				}
				break;
			}

			if (! flg) {
				u++;
				if (u > 4) { u -= 4; }
			}
		}

		mvlst[k]           = u;
		matrix[row][col]   = matrix[nrow][ncol];
		matrix[nrow][ncol] = brow * bcol - 1;
		row                = nrow;
		col                = ncol;
	}

	if (display) {
		if (scene >= 3) {
			scene = 0;
		} else {
			scene++;
		}
	}

	minb = min(bwidth, bheight);
	switch (scene) {
	case 0:
		xmov = -step;
		ymov = -step;
		xbgn =  minb - 1;
		ybgn =  minb - 1;
		break;
	case 1:
		xmov =  0;
		ymov =  0;
		xbgn =  0;
		ybgn =  0;
		break;
	case 2:
		xmov =  0;
		ymov = -step;
		xbgn =  0;
		ybgn =  minb - 1;
		break;
	case 3:
		xmov = -step;
		ymov =  0;
		xbgn =  minb - 1;
		ybgn =  0;
		break;
	}

	endc = minb + brow * 8;
	for (i = 0; i < endc; i += step) {
		if ((i % 2) == 0) {
			api_settimer(timer, 1);
			for (;;) {
				key = api_getkey(1);
				if (key ==  128) { break;    }
				if (key == 0x0a) { term = 1; }
			}
		}

		for (j = 0; j < brow; j++) {
			stat = i - j * 8;
			if (stat < 0 || minb <= stat) { continue; }

			fwidth  = bwidth  - minb + stat + 1;
			fheight = bheight - minb + stat + 1;
			fpointx = xbgn + stat * xmov;
			fpointy = ybgn + stat * ymov;

			for (k = 0; k < bcol; k++) {
				srow = matrix[j][k] / bcol;
				scol = matrix[j][k] % bcol;
				api_bitbltEx(win, pbuf,
					ltpointx + k * bwidth + fpointx, ltpointy + j * bheight + fpointy,
					scol * bwidth + fpointx, srow * bheight + fpointy, fwidth, fheight, pwidth);
			}
		}

		if (display) {
			DisplayClock();
		}
		api_refreshwin(win, 0, 0, V_WIDTH, V_HEIGHT);
	}

	for (i = 0; i < brow; i++) {
		for (j = 0; j < bcol; j++) {
			srow = matrix[i][j] / bcol;
			scol = matrix[i][j] % bcol;
			if (matrix[i][j] == brow * bcol - 1) {
				api_boxfilwin(win, ltpointx + j * bwidth, ltpointy + i * bheight,
					ltpointx + (j + 1) * bwidth - 1, ltpointy + (i + 1) * bheight - 1, 0);
			} else {
				api_bitbltEx(win, pbuf, ltpointx + j * bwidth, ltpointy + i * bheight,
					scol * bwidth, srow * bheight, bwidth, bheight, pwidth);
			}
		}
	}

	return;
}

//===================================================================
//
//	NextCut
//
//	次カットの準備をする
//
//===================================================================
void NextCut(void)
{
	int		i, j, k;
	int		step = 1, key;
	int		xmov = 0, ymov = 0;
	int		xbgn = 0, ybgn = 0;
	int		fwidth, fheight;
	int		fpointx, fpointy;
	int		minb, endc, stat;

	minb = min(bwidth, bheight);
	switch (scene) {
	case 0:
		xmov =  step;
		ymov =  step;
		xbgn =  0;
		ybgn =  0;
		break;
	case 1:
		xmov =  0;
		ymov =  0;
		xbgn =  0;
		ybgn =  0;
		break;
	case 2:
		xmov =  0;
		ymov =  step;
		xbgn =  0;
		ybgn =  0;
		break;
	case 3:
		xmov =  step;
		ymov =  0;
		xbgn =  0;
		ybgn =  0;
		break;
	}

	endc = minb + brow * 8;
	for (i = 0; i <= endc; i += step) {
		if ((i % 2) == 0) {
			api_settimer(timer, 1);
			for (;;) {
				key = api_getkey(1);
				if (key ==  128) { break;    }
				if (key == 0x0a) { term = 1; }
			}
		}

		for (j = 0; j < brow; j++) {
			stat = i - j * 8;
			if (stat < 0 || minb < stat) { continue; }

			fwidth  = bwidth  - stat;
			fheight = bheight - stat;
			fpointx = xbgn + stat * xmov;
			fpointy = ybgn + stat * ymov;

			for (k = 0; k < bcol; k++) {
				api_boxfilwin(win,
					ltpointx + k * bwidth, ltpointy + j * bheight,
					ltpointx + (k + 1) * bwidth - 1, ltpointy + (j + 1) * bheight - 1, 0);
				api_bitbltEx(win, pbuf,
					ltpointx + k * bwidth + fpointx, ltpointy + j * bheight + fpointy,
					k * bwidth + fpointx, j * bheight + fpointy, fwidth, fheight, pwidth);
			}
		}

		if (display) {
			DisplayClock();
		}
		api_refreshwin(win, 0, 0, V_WIDTH, V_HEIGHT);
	}

	api_free(pbuf, pwidth * pheight * 2);

	brow    = bmpinfo[cutcnt].brow;
	bcol    = bmpinfo[cutcnt].bcol;
	bwidth  = bmpinfo[cutcnt].bwidth;
	bheight = bmpinfo[cutcnt].bheight;
	pwidth  = bmpinfo[cutcnt].pwidth;
	pheight = bmpinfo[cutcnt].pheight;

	LoadImage();

	ltpointx = (V_WIDTH  - pwidth ) / 2;
	ltpointy = (V_HEIGHT - pheight) / 2;
	if (pwidth <= V_WIDTH - 608 || pheight <= V_HEIGHT - 64) {
		display = 1;
	} else {
		display = 0;
	}

	return;
}

//===================================================================
//
//	GetFileInfo
//
//	画像ファイルを列挙する
//
//===================================================================
int GetFileInfo(void)
{
	int		filenum;
	int		fh, fsize;
	int		info[8], rw, rh;

	sprintf(bmpinfo[0].filepath, "rapuos.bmp\0");
	sprintf(bmpinfo[1].filepath, "fujisan.jpg\0");

	filenum = 0;
	while (filenum < BMP_MAX) {
		fh = api_fopen(bmpinfo[filenum].filepath);
		if (fh == 0) { 
			break;
		}
		fsize = api_fsize(fh, 0);
		if (fsize > 512 * 1024) {
			api_fclose(fh);
			continue;
		}
		api_fread(filebuf, fsize, fh);
		api_fclose(fh);

		if (ReadBitmap(info, fsize) && info[2] >= 120 && info[3] >= 120) {
			if (info[2] > V_WIDTH ) {
				rw = V_WIDTH;
			} else {
				rw = info[2];
			}
			if (info[3] > V_HEIGHT) {
				rh = V_HEIGHT;
			} else {
				rh = info[3];
			}
			bmpinfo[filenum].type    = info[0];
			bmpinfo[filenum].size    = fsize;
			bmpinfo[filenum].brow    = (int) (rh / 80);
			if ((rh % 80) != 0) { bmpinfo[filenum].brow++; }
			bmpinfo[filenum].bcol    = (int) (rw / 80);
			if ((rw % 80) != 0) { bmpinfo[filenum].bcol++; }
			bmpinfo[filenum].bwidth  = (int) (rw / bmpinfo[filenum].bcol);
			bmpinfo[filenum].bheight = (int) (rh / bmpinfo[filenum].brow);
			bmpinfo[filenum].pwidth  = bmpinfo[filenum].bwidth  *
									   bmpinfo[filenum].bcol;
			bmpinfo[filenum].pheight = bmpinfo[filenum].bheight *
									   bmpinfo[filenum].brow;
			bmpinfo[filenum].owidth  = info[2];

			filenum++;
		}
	}

	return(filenum);
}

//===================================================================
//
//	GetFrameNum
//
//	フレーム数を取得する
//
//===================================================================
int GetFrameNum(void)
{
	fnumber = FRAME_DEF;
	mvlst = api_malloc(fnumber);

	return(1);
}

//===================================================================
//
//	DisplayClock
//
//	時計を表示する
//
//===================================================================
void DisplayClock(void)
{
	char	msg[20];

	api_systime(t);
	sprintf(msg, "%02d%02d/%02d/%02d %02d:%02d:%02d\0",
		t[6], t[5], t[4], t[3], t[2], t[1], t[0]);

	if (t[0] != osec ) {
		api_boxfilwin(win, loc[scene].x, loc[scene].y,
			loc[scene].x + 303, loc[scene].y + 31, 0);
		api_putstrwinEx(win, loc[scene].x, loc[scene].y, 6, 19, 2, msg);
		osec = t[0];
	}

	return;
}

//===================================================================
//
//	ReadBitmap
//
//	画像ファイル情報を取得する
//
//===================================================================
int ReadBitmap(int *info, int fsize)
{
	if (info_BMP(&env, info, fsize, filebuf) == 0) {
		/* BMPではなかった */
		if (info_JPEG(&env, info, fsize, filebuf) == 0) {
			/* JPEGでもなかった */
			return(0);
		}
	}

	return(1);
}

//===================================================================
//
//	LoadImage
//
//	画像ファイルをメモリに展開する
//
//===================================================================
int LoadImage(void)
{
	int		i, j;
	int		rc, fh;
	short	*p;
	struct	RGB picbuf[1024 * 768], *q;

	fh = api_fopen(bmpinfo[cutcnt].filepath);
	if (fh == 0) { 
		return(0);
	}
	api_fread(filebuf, bmpinfo[cutcnt].size, fh);
	api_fclose(fh);

	switch (bmpinfo[cutcnt].type) {
	case 1:
		rc = decode0_BMP (&env, bmpinfo[cutcnt].size, filebuf, 4, (char *) picbuf, 0);
		break;
	case 2:
		rc = decode0_JPEG(&env, bmpinfo[cutcnt].size, filebuf, 4, (char *) picbuf, 0);
		break;
	default:
		rc = -1;
		break;
	}

	if (rc != 0) {
		return(0);
	}

	pbuf = api_malloc(pwidth * pheight * 2);
	for (i = 0; i < pheight; i++) {
		p = pbuf   + i * pwidth;
		q = picbuf + i * bmpinfo[cutcnt].owidth;
		for (j = 0; j < pwidth; j++) {
			p[j] = rgb2pal(q[j].r, q[j].g, q[j].b, j, i, V_COLBIT);
		}
	}

	return(1);
}

//===================================================================
//
//	rgb2pal
//
//	表示色数に応じて減色する
//
//===================================================================
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
