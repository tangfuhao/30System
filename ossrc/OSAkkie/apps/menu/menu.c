/* ------------------------------------------------------------------ */
/*   OSAkkie Application Launcher     Ver.0.1 (2006-11-18)            */
/*   stack: 120k, malloc:0k           Copyright (C) 2006 Akkiesoft.   */
/* ------------------------------------------------------------------ */

#include "apilib.h"

static int win, timer;

#define APPS		17

/* Zakky�� � menu.c � ���� ��ݺ� � �ϼ���i�Ȃ����������p�J�^�J�i�H�j */
static struct {
	char filename[9];
	int arg;
	char filename2[22];
	char desc[5][30];
} list[17] = {
	{"bball   ", 0, "", {"OS�޻�ƭ��� 29����� bball�޽�", "�� 1��� 1��� � �ڶ���� ���Է�", "��ɼ� ��޻��                 ", "", ""}},
	{"bonno   ", 0, "", {"���ɰ ���ײ ��� ���� �޽�    ", "��߰��� � ��� ���ɰ � ���׵�!", "", "", ""}},
	{"color   ", 0, "", {"OS�޻�ƭ��� 25����� color�޽�", "����� ���� � �� �޼��ޤ �����", "�����ؤ ����� �ݺ� ��� �Ͻ�  ", "", ""}},
	{"csvv    ", 1, "csvv sample.csv", {"�ݲ CSV �ޭ�� �޽� ���� �� Ԥ", "PageUp,Down �� �޿�� �޷Ͻ�  ", "�� �ƭ� �� � ����� � �׷Ͻ� ", "������� �� �csvv ̧��Ҳ� �ֳ�", "ƭ�خ� ��� �׹Ͻ�            "}},
	{"cvtg    ", 0, "", {"����ݼ� Ĳ��� � ���� �Ķ�    ", "�޷� ���� �޽� ���� � ��ø�  ", "�� ��� � ��޳ � ���� � ����� ", "�޶� � ��� ��޴� ��޻��      ", ""}},
	{"gview   ", 1, "gview fukurou.jpg", {"OS�޻�ƭ��� 30����� �޿޳    ", "�ޭ���޽� bmp, jpeg, ico ̧��", "� �׸�Ķ� �޷Ͻ�  ������� �� ", "�gview ̧��Ҳ� � ƭ�خ� ���  ", "�׹Ͻ�"}},
	{"invader ", 0, "", {"OS�޻�ƭ��� 30����� ���ް�ް ", "�ް� �޽� ��ج� �ø� ������� ", "�� ���� � ��۳!              ", "", ""}},
	{"keyview ", 0, "", {"���� � ƭ�خ� ��� ������ �   ", "ˮ��� ���޹ � ý� ���� �޽�  ", "", "", ""}},
	{"kuha    ", 0, "", {"Ѷ� �¶� NEC PC-98x1�ذ�� �  ", "�Эڰ� �޽� ͯ�ߺ ���� ��ذ  ", "ý� ����� � ����޳ �üϲϽ�  ", "�ް��� �� �ݷ���� ��ɼи�޻��", ""}},
	{"luckynum", 0, "", {"ׯ�����ް � ׯ���װ � ˮ���  ", "�ø�� �����޽� ��޶�ϴ � ����", "���� ��޻��                  ", "", ""}},
	{"mmlplay ", 1, "mmlplay fujisan.mml", {"OS�޻�ƭ��� 30����� mml��ڰ԰", "�޽� �ް�� �� � ����� �� ��޸", "� ���ި� ���ɼ� ��޻�������� ", "", ""}},
	{"mtorz   ", 0, "", {"orz �� 10�� �� � ¸�� �Ͻ�   ", "��Ӥ �Ų� � �߶�߶ ���Է Ͻ� ", "���� ���خ� ��ø�޻��        ", "", ""}},
	{"noodle  ", 0, "", {"OS�޻�ƭ��� 24����� װ����ϰ ", "�޽� ��? ��� ����� ������ �� ", "��Ͻȡ �ެ� ���� �� ��ϰ ��  ", "¸�� �ϼ���?                 ", ""}},
	{"osakcnt ", 0, "", {"OSAkkie �� ��޳ �ö�� �޶� � ", "ˮ��� �� ���� �޽�           ", "tomo_gettick(); � ����� ���ء", "", ""}},
	{"tview   ", 1, "tview sample.txt -w42", {"OS�޻�ƭ��� 30����� ÷���ޭ��", "�޽� ����� � �� ��ݺ� ���߳  ", "������ � ֳ��ϼ��B           ", "", ""}},
	{"walk    ", 0, "", {"OS�޻�ƭ��� 23����� �� ƭ�خ�", "ý� ���� �޽� �Ӽ� � ĳ�� ���", "OSAkkie ��ʤ ŵл� �� �ٷϽ� ", "", ""}},
	{"winhello", 0, "", {"OS�޻�ƭ��� 22����� ����޳   ", "ˮ��� ý� �޽� OSAkkie ��̫��", "� ˮ��� ý� � ԯ� �ϼ��      ", "", ""}}
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
	/* �J�[�\�� */
	putline(15, py * 12 + 6, 15, py * 12 + 6, c);
	putline(14, py * 12 + 5, 14, py * 12 + 7, c);
	putline(13, py * 12 + 4, 13, py * 12 + 8, c);
	putline(12, py * 12 + 3, 12, py * 12 + 9, c);
	if (c == 0) {
		/* ���� */
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
