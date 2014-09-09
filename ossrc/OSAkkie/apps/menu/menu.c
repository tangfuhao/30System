/* ------------------------------------------------------------------ */
/*   OSAkkie Application Launcher     Ver.0.1 (2006-11-18)            */
/*   stack: 120k, malloc:0k           Copyright (C) 2006 Akkiesoft.   */
/* ------------------------------------------------------------------ */

#include "apilib.h"

static int win, timer;

#define APPS		17

/* Zakkyｻﾝ ﾉ menu.c ﾉ ｺｰﾄﾞ ｦｻﾝｺｳ ﾆ ｼﾏｼﾀ｡（なぜここも半角カタカナ？） */
static struct {
	char filename[9];
	int arg;
	char filename2[22];
	char desc[5][30];
} list[17] = {
	{"bball   ", 0, "", {"OSｼﾞｻｸﾆｭｳﾓﾝ 29ﾆﾁﾒﾖﾘ bballﾃﾞｽ｡", "ｾﾝ 1ﾎﾟﾝ 1ﾎﾟﾝ ﾉ ｴﾚｶﾞﾝﾄﾅ ｶｶﾞﾔｷｦ", "ｵﾀﾉｼﾐ ｸﾀﾞｻｲ｡                 ", "", ""}},
	{"bonno   ", 0, "", {"ﾎﾞﾝﾉｰ ｳﾁﾊﾗｲ ｼｴﾝ ｱﾌﾟﾘ ﾃﾞｽ｡    ", "ｽﾍﾟｰｽｷｰ ｦ ｵｼﾃ ﾎﾞﾝﾉｰ ｦ ｳﾁﾊﾗｵｳ!", "", "", ""}},
	{"color   ", 0, "", {"OSｼﾞｻｸﾆｭｳﾓﾝ 25ﾆﾁﾒﾖﾘ colorﾃﾞｽ｡", "ﾎﾝﾃﾞﾊ ｱｶｹｲ ﾉ ｲﾛ ﾃﾞｼﾀｶﾞ､ ｱｯｷｨﾉ", "ｼｭﾐﾆﾖﾘ､ ｱｵｹｲﾆ ﾍﾝｺｳ ｻﾚﾃ ｲﾏｽ｡  ", "", ""}},
	{"csvv    ", 1, "csvv sample.csv", {"ｶﾝｲ CSV ﾋﾞｭｰｱ ﾃﾞｽ｡ ｶｰｿﾙ ｷｰ ﾔ､", "PageUp,Down ｷｰ ﾃﾞｿｳｻ ﾃﾞｷﾏｽ｡  ", "ｺﾉ ﾒﾆｭｰ ｶﾗ ﾊ ｻﾝﾌﾟﾙ ｦ ﾋﾗｷﾏｽ｡ ", "ﾌﾟﾛﾝﾌﾟﾄ ｶﾗ ｢csvv ﾌｧｲﾙﾒｲ｣ ﾉﾖｳﾆ", "ﾆｭｳﾘｮｸ ｼﾃﾓ ﾋﾗｹﾏｽ｡            "}},
	{"cvtg    ", 0, "", {"ｼｭｳﾍﾝｼﾔ ﾄｲｳﾓﾉ ｦ ｷﾀｴﾙ ｺﾄｶﾞ    ", "ﾃﾞｷﾙ ｱﾌﾟﾘ ﾃﾞｽ｡ ｶﾞﾒﾝ ﾆ ﾃﾞﾃｸﾙ  ", "ﾏﾙ ｼｶｸ ﾉ ｷｺﾞｳ ﾄ ｽｳｼﾞ ｦ ｾｲｹﾞﾝ ", "ｼﾞｶﾝ ﾉ ｳﾁﾆ ｵﾎﾞｴﾃ ｸﾀﾞｻｲ｡      ", ""}},
	{"gview   ", 1, "gview fukurou.jpg", {"OSｼﾞｻｸﾆｭｳﾓﾝ 30ﾆﾁﾒﾖﾘ ｶﾞｿﾞｳ    ", "ﾋﾞｭｰｱﾃﾞｽ｡ bmp, jpeg, ico ﾌｧｲﾙ", "ｦ ﾋﾗｸｺﾄｶﾞ ﾃﾞｷﾏｽ｡  ﾌﾟﾛﾝﾌﾟﾄ ｶﾗ ", "｢gview ﾌｧｲﾙﾒｲ｣ ﾄ ﾆｭｳﾘｮｸ ｼﾃﾓ  ", "ﾋﾗｹﾏｽ｡"}},
	{"invader ", 0, "", {"OSｼﾞｻｸﾆｭｳﾓﾝ 30ﾆﾁﾒﾖﾘ ｲﾝﾍﾞｰﾀﾞｰ ", "ｹﾞｰﾑ ﾃﾞｽ｡ ｼﾝﾘｬｸ ｼﾃｸﾙ ｳﾁｭｳｼﾞﾝ ", "ｶﾗ ﾁｷｭｳ ｦ ﾏﾓﾛｳ!              ", "", ""}},
	{"keyview ", 0, "", {"ｱﾌﾟﾘ ﾆ ﾆｭｳﾘｮｸ ｻﾚﾀ ｷｰｺｰﾄﾞ ｦ   ", "ﾋｮｳｼﾞ ｽﾙﾀﾞｹ ﾉ ﾃｽﾄ ｱﾌﾟﾘ ﾃﾞｽ｡  ", "", "", ""}},
	{"kuha    ", 0, "", {"ﾑｶｼ ﾅﾂｶｼ NEC PC-98x1ｼﾘｰｽﾞ ﾉ  ", "ｴﾐｭﾚｰﾀ ﾃﾞｽ｡ ﾍｯﾎﾟｺ ﾅﾉﾃﾞ ﾒﾓﾘｰ  ", "ﾃｽﾄ ﾁｮｸｺﾞ ﾆ ｻｲｷﾄﾞｳ ｼﾃｼﾏｲﾏｽ｡  ", "ﾋﾞｰﾌﾟﾉ ﾅﾙ ｶﾝｷｮｳﾃﾞ ｵﾀﾉｼﾐｸﾀﾞｻｲ｡", ""}},
	{"luckynum", 0, "", {"ﾗｯｷｰﾅﾝﾊﾞｰ ﾄ ﾗｯｷｰｶﾗｰ ｦ ﾋｮｳｼﾞ  ", "ｼﾃｸﾚﾙ ｱﾌﾟﾘﾃﾞｽ｡ ｵﾃﾞｶｹﾏｴ ﾆ ﾁｪｯｸ", "ｼﾃﾐﾃ ｸﾀﾞｻｲ｡                  ", "", ""}},
	{"mmlplay ", 1, "mmlplay fujisan.mml", {"OSｼﾞｻｸﾆｭｳﾓﾝ 30ﾆﾁﾒﾖﾘ mmlﾌﾟﾚｰﾔｰ", "ﾃﾞｽ｡ ﾋﾞｰﾌﾟ ｵﾝ ﾉ ｼﾝﾌﾟﾙ ﾃﾞ ｿﾎﾞｸ", "ﾅ ﾒﾛﾃﾞｨｰ ｦｵﾀﾉｼﾐ ｸﾀﾞｻｲ･･････｡ ", "", ""}},
	{"mtorz   ", 0, "", {"orz ｶﾞ 10ﾆﾝ ﾔﾏ ｦ ﾂｸｯﾃ ｲﾏｽ｡   ", "ｼｶﾓ､ ﾅﾅｲﾛ ﾆ ﾋﾟｶﾋﾟｶ ｶｶﾞﾔｷ ﾏｽ｡ ", "ｱｷﾀﾗ ｼｭｳﾘｮｳ ｻｾﾃｸﾀﾞｻｲ｡        ", "", ""}},
	{"noodle  ", 0, "", {"OSｼﾞｻｸﾆｭｳﾓﾝ 24ﾆﾁﾒﾖﾘ ﾗｰﾒﾝﾀｲﾏｰ ", "ﾃﾞｽ｡ ｱﾗ? ｿｺﾆ ﾁｮｳﾄﾞ ｶｯﾌﾟﾒﾝ ｶﾞ ", "ｱﾘﾏｽﾈ｡ ｼﾞｬｱ ﾁｮｯﾄ ｺﾉ ﾀｲﾏｰ ﾃﾞ  ", "ﾂｸｯﾃ ﾐﾏｼｮｳﾖ?                 ", ""}},
	{"osakcnt ", 0, "", {"OSAkkie ｶﾞ ｷﾄﾞｳ ｼﾃｶﾗﾉ ｼﾞｶﾝ ｦ ", "ﾋｮｳｼﾞ ｽﾙ ｱﾌﾟﾘ ﾃﾞｽ｡           ", "tomo_gettick(); ﾉ ｻﾝﾌﾟﾙ ｱﾌﾟﾘ｡", "", ""}},
	{"tview   ", 1, "tview sample.txt -w42", {"OSｼﾞｻｸﾆｭｳﾓﾝ 30ﾆﾁﾒﾖﾘ ﾃｷｽﾄﾋﾞｭｰｱ", "ﾃﾞｽ｡ ｻﾝﾌﾟﾙ ﾄ ｼﾃ ﾆﾎﾝｺｸ ｹﾝﾎﾟｳ  ", "ｾﾞﾝﾌﾞﾝ ｦ ﾖｳｲｼﾏｼﾀ。           ", "", ""}},
	{"walk    ", 0, "", {"OSｼﾞｻｸﾆｭｳﾓﾝ 23ﾆﾁﾒﾖﾘ ｷｰ ﾆｭｳﾘｮｸ", "ﾃｽﾄ ｱﾌﾟﾘ ﾃﾞｽ｡ ｴﾓｼﾞ ｦ ﾄｳｻｲ ｼﾃﾙ", "OSAkkie ﾃﾞﾊ､ ﾅｵﾐｻﾝ ｶﾞ ｱﾙｷﾏｽ｡ ", "", ""}},
	{"winhello", 0, "", {"OSｼﾞｻｸﾆｭｳﾓﾝ 22ﾆﾁﾒﾖﾘ ｳｨﾝﾄﾞｳ   ", "ﾋｮｳｼﾞ ﾃｽﾄ ﾃﾞｽ｡ OSAkkie ﾐﾆﾌｫﾝﾄ", "ﾉ ﾋｮｳｼﾞ ﾃｽﾄ ﾓ ﾔｯﾃ ﾐﾏｼﾀ｡      ", "", ""}}
};

void splitestr(int px, int py, int l, char *s, int r);
void putline(int spx, int spy, int epx, int epy, int c);
void putcursor(int py, int c);
void wait(int time);

void HariMain(void)
{
	char buf[400 * 300];
	int i, cursor;

	tomo_setlang(0);
	win = api_openwin(buf, 400, 300, -1, "Welcome to OSAkkie.");
	api_boxfilwin(win,  6, 28, 392, 292, 7);
	timer = api_alloctimer();
	api_inittimer(timer, 128);

	putline(0, 25, 386, 25, 0);
	putline(90, 25, 90, 264, 0);

	wait(25);
	splitestr(1,  1, 37, "OSAkkie application launcher  Ver.0.1", 3);
	wait(50);

	for (i = 0; i < APPS; i++) {
		splitestr(3, i + 3, 8, list[i].filename, 2);
	}
	splitestr(17,  3, 13, "Description:", 3);
	putcursor(3, 0);	/* bball */

	cursor = 3;

	for (;;) {
		i = api_getkey(1);
		if (i == 0x48) {
			putcursor(cursor, 7);
			if (cursor == 3) {
				cursor = 20;
			} else {
			}
			cursor--;
			putcursor(cursor, 0);
		}
		if (i == 0x50) {
			putcursor(cursor, 7);
			if (cursor == 19) {
				cursor = 2;
			}
			cursor++;
			putcursor(cursor, 0);
		}
		if (i == 0x0a) {
			if (list[cursor - 3].arg == 0) {
				osak_exec(list[cursor - 3].filename);
			} else {
				osak_exec(list[cursor - 3].filename2);
			}
		}
	}
	api_closewin(win);
	api_end();
}

void splitestr(int px, int py, int l, char *s, int r)
{
	int i;
	for (i = 0; i < r; i++) {
		wait(3);
		osak_putministrwin(win, 6 + px * 6, 28 + py * 12, 7, l, s);
		wait(3);
		osak_putministrwin(win, 6 + px * 6, 28 + py * 12, 0, l, s);
	}
	return;
}

void putline(int spx, int spy, int epx, int epy, int c)
{
	api_linewin(win, 6 + spx, 28 + spy, 6 + epx, 28 + epy, c);
	return;
}

void putcursor(int py, int c)
{
	int i;
	/* カーソル */
	putline(15, py * 12 + 6, 15, py * 12 + 6, c);
	putline(14, py * 12 + 5, 14, py * 12 + 7, c);
	putline(13, py * 12 + 4, 13, py * 12 + 8, c);
	putline(12, py * 12 + 3, 12, py * 12 + 9, c);
	if (c == 0) {
		/* 説明 */
		py -= 3;
		api_boxfilwin(win, 108, 88, 288, 148, 7);
		for (i = 0; i < 5; i++) {
			if (list[py].desc[i][0] == 0) {
				break;
			}
			splitestr(17,  i + 5, 29, list[py].desc[i], 1);
		}
	}
	return;
}

void wait(int time)
{
	api_settimer(timer, time);
	while (api_getkey(1) != 128);
	return;
}
