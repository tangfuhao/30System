/* ウィンドウ関係 */

#include "bootpack.h"

extern unsigned short table_8_565[256];

void make_window8(unsigned short *buf, int xsize, int ysize, char *title, char act)
{
	boxfill8(buf, xsize, COL8_C6C6C6, 0,         0,         xsize - 1, 0        );
	boxfill8(buf, xsize, COL8_FFFFFF, 1,         1,         xsize - 2, 1        );
	boxfill8(buf, xsize, COL8_C6C6C6, 0,         0,         0,         ysize - 1);
	boxfill8(buf, xsize, COL8_FFFFFF, 1,         1,         1,         ysize - 2);
	boxfill8(buf, xsize, COL8_848484, xsize - 2, 1,         xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, xsize - 1, 0,         xsize - 1, ysize - 1);
	boxfill8(buf, xsize, COL8_C6C6C6, 2,         2,         xsize - 3, ysize - 3);
	boxfill8(buf, xsize, COL8_848484, 1,         ysize - 2, xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, 0,         ysize - 1, xsize - 1, ysize - 1);
	make_wtitle8(buf, xsize, title, act);
	return;
}

void make_menu8(unsigned short *buf, int xsize, int ysize, char *title, struct MENU *menu, int num)
{
	int i, yp, col = 16 + 2 + 2 * 6 + 2 * 36;
	struct MENU *pmenu;

	boxfill8(buf, xsize, COL8_C6C6C6, 0,         0,         xsize - 1, 0        );
	boxfill8(buf, xsize, COL8_FFFFFF, 1,         1,         xsize - 2, 1        );
	boxfill8(buf, xsize, COL8_C6C6C6, 0,         0,         0,         ysize - 1);
	boxfill8(buf, xsize, COL8_FFFFFF, 1,         1,         1,         ysize - 2);
	boxfill8(buf, xsize, COL8_848484, xsize - 2, 1,         xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, xsize - 1, 0,         xsize - 1, ysize - 1);
	boxfill8(buf, xsize, COL8_C6C6C6, 2,         2,         xsize - 3, ysize - 3);
	boxfill8(buf, xsize, COL8_848484, 1,         ysize - 2, xsize - 2, ysize - 2);
	boxfill8(buf, xsize, COL8_000000, 0,         ysize - 1, xsize - 1, ysize - 1);

	pmenu = menu;
	for (i = 0; i < num; i++) {
		putfonts8_asc(buf, xsize, 8, ysize - (2 + 26 * (i + 1)) + 6, COL8_000000, 1, pmenu->name);
		pmenu = pmenu->next;
	}
	for (i = 0; i < num - 1; i++) {
		yp = ysize - (2 + 26 * (i + 1));
		boxfill8(buf, xsize, col,         4, yp,     xsize - 6, yp    );
		boxfill8(buf, xsize, COL8_FFFFFF, 5, yp + 1, xsize - 5, yp + 1);
	}

	if (menu->level == 0) {
		make_mtitle8(buf, xsize, title, 1);
	}

	return;
}

void make_wtitle8(unsigned short *buf, int xsize, char *title, char act)
{
	static char ctrlbtn[15][15] = {
		"###############",
		"-OOOOOOOOOOOOOO",
		"#############--",
		"-OOOOOOOOOOOO--",
		"###########----",
		"-OOOOOOOOOO----",
		"#########------",
		"-OOOOOOOO------",
		"#######--------",
		"-OOOOOO--------",
		"#####----------",
		"-OOOO----------",
		"###------------",
		"-OO------------",
		"#--------------"
	};
	//	 ----+----1----+----2----+----3----+----4----+----5----+---
	static char closebtn[14][58] = {
		"------------############Q$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$O",
		"------------OOOOOOOOOOOO$@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@OO",
		"----------############Q$@OQQQQQQQQQQQQQ@OQQQQQQQQQQQQQ@O#-",
		"----------OOOOOOOOOOOO$@QQ$$QQQQQQOOO$@QQ$$QQQQQQOOO$@OOOO",
		"--------############Q$@QQ$QQQQQQQOOO$@Q##QQQQQ###OO$@O###-",
		"--------OOOOOOOOOOOO$@QQQQQQQQQQOOO$@QQQ##QQ###OOO$@OOOOOO",
		"------############Q$@QQQQQQQQQQOOO$@QQQQQ####QOOO$@O#####-",
		"------OOOOOOOOOOOO$@QQQQQQQQQQOOO$@QQQQQ####QOOO$@OOOOOOOO",
		"----############Q$@QQ#########OO$@QQQQ###QQ##OO$@O#######-",
		"----OOOOOOOOOOOO$@QO#########OO$@QOO###OOOOO##$@OOOOOOOOOO",
		"--############Q$@QOOOOOOOOOOOO$@QOOOOOOOOOOOO$@O#########-",
		"--OOOOOOOOOOOO$@Q$$$$$$$$$$$$$@Q$$$$$$$$$$$$$@OOOOOOOOOOOO",
		"############Q$@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@O###########-",
		"OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"
	};
	int x, y;
	unsigned char c, tc, tbc;
	if (act != 0) {
		tc = COL8_FFFFFF;
		tbc = 16 + 1 + 2 * 6 + 5 * 36;
	} else {
		tc = COL8_FFFFFF;
		tbc = COL8_848484;
	}
	boxfill8(buf, xsize, tbc, 5, 4, xsize - 6, 19);
	putfonts8_asc(buf, xsize, 24, 4, tc, 1, title);
	for (y = 0; y < 15; y++) {
		for (x = 0; x < 15; x++) {
			c = ctrlbtn[y][x];
			if (c == 'O') {
				c = COL8_FFFFFF;
			} else if (c == '#') {
				c = 16 + 2 + 2 * 6 + 2 * 36;
			} else {
				c = 0xff;
			}
			if (c != 0xff) {
				buf[(4 + y) * xsize + (4 + x)] = table_8_565[c];
			}
		}
	}
	for (y = 0; y < 14; y++) {
		for (x = 0; x < 58; x++) {
			c = closebtn[y][x];
			if (c == '@') {
				c = COL8_000000;
			} else if (c == '$') {
				c = 16 + 3 + 3 * 6 + 3 * 36;
			} else if (c == 'Q') {
				c = 16 + 4 + 4 * 6 + 4 * 36;
			} else if (c == 'O') {
				c = COL8_FFFFFF;
			} else if (c == '#') {
				c = 16 + 2 + 2 * 6 + 2 * 36;
			} else {
				c = 0xff;
			}
			if (c != 0xff) {
				buf[(4 + y) * xsize + (xsize - 62 + x)] = table_8_565[c];
			}
		}
	}
	return;
}

void make_mtitle8(unsigned short *buf, int xsize, char *title, char act)
{
	static char ctrlbtn[16][16] = {
		"################",
		"-OOOOOOOOOOOOOOO",
		"##############--",
		"-OOOOOOOOOOOOO--",
		"############----",
		"-OOOOOOOOOOO----",
		"##########------",
		"-OOOOOOOOO------",
		"########--------",
		"-OOOOOOO--------",
		"######----------",
		"-OOOOO----------",
		"####------------",
		"-OOO------------",
		"##--------------",
		"-O--------------"
	};
	//	 ----+----1----+-
	static char closebtn[16][16] = {
		"--------------#-",
		"--------------OO",
		"------------###-",
		"------------OOOO",
		"----------#####-",
		"----------OOOOOO",
		"--------#######-",
		"--------OOOOOOOO",
		"------#########-",
		"------OOOOOOOOOO",
		"----###########-",
		"----OOOOOOOOOOOO",
		"--#############-",
		"--OOOOOOOOOOOOOO",
		"###############-",
		"OOOOOOOOOOOOOOOO"
	};
	int x, y;
	unsigned char c, tc, tbc;
	if (act != 0) {
		tc = COL8_FFFFFF;
		tbc = 16 + 1 + 2 * 6 + 5 * 36;
	} else {
		tc = COL8_FFFFFF;
		tbc = COL8_848484;
	}
	boxfill8(buf, xsize, tbc, 5, 4, xsize - 6, 19);
	putfonts8_asc(buf, xsize, 24, 4, tc, 1, title);
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			c = ctrlbtn[y][x];
			if (c == 'O') {
				c = COL8_FFFFFF;
			} else if (c == '#') {
				c = 16 + 2 + 2 * 6 + 2 * 36;
			} else {
				c = 0xff;
			}
			if (c != 0xff) {
				buf[(4 + y) * xsize + (4 + x)] = table_8_565[c];
			}
		}
	}
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			c = closebtn[y][x];
			if (c == 'O') {
				c = COL8_FFFFFF;
			} else if (c == '#') {
				c = 16 + 2 + 2 * 6 + 2 * 36;
			} else {
				c = 0xff;
			}
			if (c != 0xff) {
				buf[(4 + y) * xsize + (xsize - 20 + x)] = table_8_565[c];
			}
		}
	}
	return;
}

void putfouts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l)
{
	struct TASK *task = task_now();
	boxfill8(sht->buf, sht->bxsize, b, x, y, x + l * 8 - 1, y + 15);
	if (task->langmode != 0 && task->langbyte1 != 0) {
		putfonts8_asc(sht->buf, sht->bxsize, x, y, c, 1, s);
		sheet_refresh(sht, x - 8, y, x + l * 8, y + 16);
	} else {
		putfonts8_asc(sht->buf, sht->bxsize, x, y, c, 1, s);
		sheet_refresh(sht, x, y, x + l * 8, y + 16);
	}
	return;
}

void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c)
{
	int x1 = x0 + sx, y1 = y0 + sy;
	boxfill8(sht->buf, sht->bxsize, COL8_848484, x0 - 2, y0 - 3, x1 + 1, y0 - 3);
	boxfill8(sht->buf, sht->bxsize, COL8_848484, x0 - 3, y0 - 3, x0 - 3, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, COL8_FFFFFF, x0 - 3, y1 + 2, x1 + 1, y1 + 2);
	boxfill8(sht->buf, sht->bxsize, COL8_FFFFFF, x1 + 2, y0 - 3, x1 + 2, y1 + 2);
	boxfill8(sht->buf, sht->bxsize, COL8_000000, x0 - 1, y0 - 2, x1 + 0, y0 - 2);
	boxfill8(sht->buf, sht->bxsize, COL8_000000, x0 - 2, y0 - 2, x0 - 2, y1 + 0);
	boxfill8(sht->buf, sht->bxsize, COL8_C6C6C6, x0 - 2, y1 + 1, x1 + 0, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, COL8_C6C6C6, x1 + 1, y0 - 2, x1 + 1, y1 + 1);
	boxfill8(sht->buf, sht->bxsize, c,           x0 - 1, y0 - 1, x1 + 0, y1 + 0);
	return;
}

void change_wtitle8(struct SHEET *sht, char act)
{
	int x, y, xsize = sht->bxsize;
	unsigned char tc_new, tbc_new, tc_old, tbc_old;
	unsigned short c565, *buf = sht->buf;
	if (act != 0) {
		tc_new  = COL8_FFFFFF;
		tbc_new = 16 + 1 + 2 * 6 + 5 * 36;
		tc_old  = COL8_FFFFFF;
		tbc_old = COL8_848484;
	} else {
		tc_new  = COL8_FFFFFF;
		tbc_new = COL8_848484;
		tc_old  = COL8_FFFFFF;
		tbc_old = 16 + 1 + 2 * 6 + 5 * 36;
	}
	for (y = 4; y <= 19; y++) {
		for (x = 5; x <= xsize - 6; x++) {
			c565 = buf[y * xsize + x];
			if (c565 == table_8_565[tc_old] && x <= xsize - 63) {
				c565 = table_8_565[tc_new];
			} else if (c565 == table_8_565[tbc_old]) {
				c565 = table_8_565[tbc_new];
			}
			buf[y * xsize + x] = c565;
		}
	}
	sheet_refresh(sht, 5, 4, xsize, 20);
	return;
}

void change_mtitle8(struct SHEET *sht, int level, int mn_flg, char act)
{
	int x, y, yp, xsize = sht->bxsize;
	int ys, ye;
	unsigned char tc_new, tbc_new, tc_old, tbc_old;
	unsigned short c565, *buf = sht->buf;
	if (act != 0) {
		tc_new  = COL8_FFFFFF;
		tbc_new = COL8_0000FF;
		tc_old  = COL8_000000;
		tbc_old = COL8_C6C6C6;
	} else {
		tc_new  = COL8_000000;
		tbc_new = COL8_C6C6C6;
		tc_old  = COL8_FFFFFF;
		tbc_old = COL8_0000FF;
	}
	yp = (mn_flg -1) * 26 + 3;
	if (level == 0) {
		yp += 18;
	}
	for (y = 0; y < 22; y++) {
		for (x = 5; x <= xsize - 6; x++) {
			c565 = buf[(yp + y) * xsize + x];
			if (c565 == table_8_565[tc_old]) {
				c565 = table_8_565[tc_new];
			} else if (c565 == table_8_565[tbc_old]) {
				c565 = table_8_565[tbc_new];
			}
			buf[(yp + y) * xsize + x] = c565;
		}
	}
	sheet_refresh(sht, 5, yp, xsize - 5, yp + 22);
	return;
}
