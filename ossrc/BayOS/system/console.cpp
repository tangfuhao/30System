#include "../bayos.h"
//#include "../sys/font_latin1.h"
#include "../sys/font_hankaku.h"
#include <stdio.h>

/* 背景色（白）*/
#define cb 0xffff
/* 前景色（黒）*/
#define cc 0x0000

#define INSETS_LEFT   6
#define INSETS_TOP    22
#define INSETS_BOTTOM 6
#define INSETS_RIGHT  6

/* 描画開始位置 */
int    Console::cx      = INSETS_LEFT;
int    Console::cy      = INSETS_TOP;
int    Console::cw      = FONT_W * 58 + INSETS_LEFT + INSETS_RIGHT;
int    Console::ch      = FONT_H * 17 + INSETS_TOP  + INSETS_BOTTOM;
word*  Console::vram    = NULL;
char*  Console::cmd_buf = NULL;
char*  Console::prt_buf = NULL;
Sheet* Console::sheet   = NULL;

/* 初期化する */
void Console::init()
{
	/* コマンドバッファーを初期化 */
	cmd_buf = new char[128];

	/* printf用バッファーを初期化 */
	prt_buf = new char[128];

	/* 下敷きを初期化 */
	sheet = SheetCtl::allocate();
	SheetCtl::setbuf(sheet, (word *) malloc (cw * ch * sizeof(short)), cw, ch, -1);
	sheet->vx0 = (SheetCtl::ctl->xsize - cw) / 2;
	sheet->vy0 = (SheetCtl::ctl->ysize - ch) / 2;
	vram       = sheet->buffer;

	/* 枠を描画する */
	SheetCtl::drawFrame(sheet, "console");

	/* コンソール領域を背景色で塗りつぶす */
	SheetCtl::fillRect(
		sheet, 
		INSETS_LEFT, 
		INSETS_TOP, 
		cw - INSETS_LEFT - INSETS_RIGHT, 
		ch - INSETS_TOP  - INSETS_BOTTOM, 
		cb
	);

	/* 下敷きリストに追加 */
	SheetCtl::updown(sheet, 1);
	
	putString("> "); /* プロンプトを描画*/
	drawCarret();    /* キャレットを描画 */
}

/* 描画開始位置を変更する */
void Console::locate(int x, int y)
{
	cx = x;
	cy = y;
	if (cx >= cw - INSETS_RIGHT) {
		cy += FONT_H;
		cx = INSETS_LEFT;
	}
	if (cy >= ch - INSETS_BOTTOM) {
		cy = ch - INSETS_BOTTOM - FONT_H;
		cx = INSETS_LEFT;
		
		/* 1行スクロール */
		memcpy(
			&vram[cw * INSETS_TOP], 
			&vram[cw * (INSETS_TOP + FONT_H)], 
			cw * (ch - 28 - FONT_H) * sizeof(short)
		);
		
		/* スクロール後の最終行を背景色で塗りつぶす */
		SheetCtl::fillRect(
			sheet, 
			INSETS_LEFT, 
			ch - INSETS_BOTTOM - FONT_H, 
			cw - INSETS_LEFT - INSETS_RIGHT, 
			FONT_H, 
			cb
		);
		SheetCtl::refresh(sheet, 0, 0, cw, ch);
	}
}

/* コマンドを解析する */
void Console::parseCommand()
{
	char temp[64];

	/* help, ? */
	if (strncmp(cmd_buf, "help", 4) == 0 || strncmp(cmd_buf, "?", 1) == 0) {
		putString("CUI ｺﾝｿｰﾙ ｺﾏﾝﾄﾞ ｲﾁﾗﾝ\n");
		putString("  help/?    - ﾍﾙﾌﾟ\n");
		putString("  ls/dir    - ﾌｧｲﾙ ｲﾁﾗﾝ ﾋｮｳｼﾞ\n");
		//putString("  cd        - ﾃﾞｨﾚｸﾄﾘ ｲﾄﾞｳ\n");
		putString("  cat/type  - ﾌｧｲﾙ ﾅｲﾖｳ ﾋｮｳｼﾞ\n");
		putString("  date/time - ｹﾞﾝｻﾞｲ ｼﾞｺｸ ﾋｮｳｼﾞ\n");
		putString("  uname/ver - ﾊﾞｰｼﾞｮﾝ ﾋｮｳｼﾞ\n");
		putString("  clear/cls - ｶﾞﾒﾝ ｸﾘｱ\n");
		putString("  ps        - ﾌﾟﾛｾｽ ｲﾁﾗﾝ ﾋｮｳｼﾞ\n");
		putString("  mem       - ﾒﾓﾘｰ ｼﾖｳﾘｮｳ ﾋｮｳｼﾞ\n");
		//putString("  kill      - ﾌﾟﾛｾｽ ｷｮｳｾｲ ｼｭｳﾘｮｳ\n");
		putString("  exit/quit - ｼｭｳﾘｮｳ\n");
	}

	/* ls, dir */
	else if (strncmp(cmd_buf, "ls", 2) == 0 || strncmp(cmd_buf, "dir", 3) == 0) {
		List<FileEntry*>* list = File::list();
		int I = list->size();
		int total_size = 0;
		for (int i = 0; i < I; i++) {
			FileEntry* info = list->get(i);
			memset(temp, '\0', sizeof(temp));
			sprintf(temp, "%8d %13s\n", info->size, info->name);
			total_size += info->size;
			//delete(info);
			putString(temp);
		}
		//delete(list);
		memset(temp, '\0', sizeof(temp));
		sprintf(temp, "    %3d ｺﾉﾌｧｲﾙ %8d ﾊﾞｲﾄ\n", I, total_size);
		putString(temp);
	}

	/* cat, type */
	else if (strncmp(cmd_buf, "cat ", 4) == 0 || strncmp(cmd_buf, "type ", 5) == 0) {
		char* filename;
		if (strncmp(cmd_buf, "cat ", 4) == 0) {
			filename = &cmd_buf[4];
		} else {
			filename = &cmd_buf[5];
		}
		if (strlen(filename) > 0) {
			FileEntry* entry = File::load(filename);
			dword read_size  = entry->size;
			if (read_size > 0) {
				for (int i = 0; i < (int) read_size; i++) {
					if (entry->data[i] == '\r') {
						// do nothing
					} else {
						putChar(entry->data[i]);
					}
				}
			}
			putChar('\n');
			//delete(entry);
		}
	}

	/* date, time */
	else if (strncmp(cmd_buf, "date", 4) == 0 || strncmp(cmd_buf, "time", 4) == 0) {
		const char* day [] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
		const char* ampm[] = { "AM", "PM" };

		RTC::init(); /* リアルタイムクロックの初期化 */
		
		/* ツェラーの公式 */
		int day_of_week = (
			RTC::year + 
			int(RTC::year/4) - 
			int(RTC::year/100) + 
			int(RTC::year/400) + 
			int((13 * RTC::month + 8)/5) + 
			RTC::day
		) % 7;

		memset(temp, 0, sizeof(temp));
		sprintf(temp, "%04d/%02d/%02d (%s) %s %02d:%02d:%02d\n",
			RTC::year, 
			RTC::month, 
			RTC::day, 
			day[day_of_week % 7],
			ampm[RTC::hour / 12], 
			RTC::hour % 12, 
			RTC::minutes, RTC::second
		);
		putString(temp);
	}

	/* uname, ver */
	else if (strncmp(cmd_buf, "uname", 5) == 0 || strncmp(cmd_buf, "ver", 3) == 0) {
		memset(temp, '\0', sizeof(temp));
		sprintf(temp, "%s\n", BAYOS_VERSION);
		putString(temp);
	}

	/* clear, cls */
	else if (strncmp(cmd_buf, "clear", 5) == 0 || strncmp(cmd_buf, "cls", 3) == 0) {
		locate(INSETS_LEFT, INSETS_TOP);
		SheetCtl::fillRect(
			sheet, 
			INSETS_LEFT, 
			INSETS_TOP, 
			cw - INSETS_LEFT - INSETS_RIGHT, 
			ch - INSETS_TOP  - INSETS_BOTTOM, 
			cb
		);
		SheetCtl::refresh(sheet, 0, 0, cw, ch);
		putString("> ");
		return;
	}

	/* ps */
	else if (strncmp(cmd_buf, "ps", 2) == 0) {
		putString("[tid]  [eip]    [esp]    [name]\n");
		for (int i = 0; i < Scheduler::runq->size(); i++) {
			Task* task = Scheduler::runq->get(i);
			memset(temp, 0, sizeof(temp));
			sprintf(temp, "%05d %08x %08x %s\n", 
				task->selector,
				task->tss.eip,
				task->tss.esp,
				task->name
			);
			putString(temp);
		}
	}

	/* mem */
	else if (strncmp(cmd_buf, "mem", 3) == 0) {
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "ｾﾞﾝﾌﾞﾂﾘﾒﾓﾘ : %8d ﾊﾞｲﾄ\n", g_km.getTotalSize());
		putString(temp);
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "    ｱｷｻｲｽﾞ : %8d ﾊﾞｲﾄ\n", g_km.getFreeSize());
		putString(temp);
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "ｼﾖｳｽﾞﾐｻｲｽﾞ : %8d ﾊﾞｲﾄ\n", (g_km.getTotalSize() - g_km.getFreeSize()));
		putString(temp);
	}

	/* exit, quit */
	else if (strncmp(cmd_buf, "exit", 4) == 0 || strncmp(cmd_buf, "quit", 4) == 0) {
		locate(0, 0);
		memset(vram, cb, cw * ch);
		SheetCtl::refresh(sheet, 0, 0, cw, ch);
		putString("ｺﾝﾋﾟｭｰﾀｰ ﾉ ﾃﾞﾝｹﾞﾝ ｦ ｷﾙ ｼﾞｭﾝﾋﾞ ｶﾞ ﾃﾞｷﾏｼﾀ.\n");
		putString("ﾃﾞﾝｹﾞﾝ ﾎﾞﾀﾝ ｦ ｵｼﾃ ｸﾀﾞｻｲ.\n");
		io_cli();
		for (;;) {
			io_hlt();
		}
	}

	/* 外部アプリ実行 */
	else {
		dword app_size, app_size1 = 0, app_size2 = 0;
		byte  *code1 = 0, *code2 = 0;
		
		/* hello.hrb */
		FileEntry* entry1 = File::load(cmd_buf);
		if (entry1 != NULL) {
			app_size1 = entry1->size;
			code1     = entry1->data;
		}
		
		/* hello */
		strcat(cmd_buf, ".hrb");
		FileEntry* entry2 = File::load(cmd_buf);
		if (entry2 != NULL) {
			app_size2 = entry2->size;
			code2     = entry2->data;
		}
		
		if ((app_size1 >= 36 && strncmp((char *) (code1 + 4), "Hari", 4) == 0 && *code1 == 0x00) ||
			(app_size2 >= 36 && strncmp((char *) (code2 + 4), "Hari", 4) == 0 && *code2 == 0x00))
		{
			/* HRBバイナリ解析 */
			byte* code;
			if (app_size1 >= 36 && strncmp((char *) (code1 + 4), "Hari", 4) == 0 && *code1 == 0x00) {
				app_size = app_size1;
				code = code1;
			} else {
				app_size = app_size2;
				code = code2;
			}
			int segment_size = *((int *) (code + 0x0000));
			int esp          = *((int *) (code + 0x000c));
			int data_size    = *((int *) (code + 0x0010));
			int data_hrb     = *((int *) (code + 0x0014));
			int malloc_start = *((int *) (code + 0x0020));
			
			/* デバッグ用 */
			//Console::printf("%d,%x,%d,%x,%x\n", segment_size, esp, data_size, data_hrb, malloc_start);
			
			/* データセグメントの確保とアプリ用メモリマネージャの初期化 */
			byte* data = (byte *)malloc(segment_size);
			Task* current = Scheduler::current;
			current->ds_base = (int) data;
			current->memory  = new Memory();
			current->memory->init((dword) &data[malloc_start], (dword) &data[segment_size - 1]);
			
			/* DSとCSの設定 */
			//SEGMENT_DESCRIPTOR* gdt = (SEGMENT_DESCRIPTOR *) ADDRESS_GDT;
			Descriptor::setSegmentDescriptor
				(&current->ldt[0], app_size - 1, (int) code, AR_CODE32_ER + 0x60);
				//(gdt + 1003, app_size - 1, (int) code, AR_CODE32_ER + 0x60);
			Descriptor::setSegmentDescriptor
				(&current->ldt[1], segment_size - 1, (int) data, AR_DATA32_RW + 0x60);
				//(gdt + 1004, segment_size - 1, (int) data, AR_DATA32_RW + 0x60);
			memcpy(&data[esp], &code[data_hrb], data_size);
			
			/* 実行開始 */
			start_app(0x1b, 0 * 8 + 4, esp, 1 * 8 + 4, &(current->tss.esp0));
			//start_app(0x1b, 1003 * 8, esp, 1004 * 8, &(current->tss.esp0));
			
			/* 実行終了 */
			//delete(entry1);
			//delete(entry2);
			delete(current->memory);
			free(data);
			//free(code);
		}
		
		/* コマンドエラー */
		else {
			Console::printf("%s: ｺﾏﾝﾄﾞｴﾗｰｷﾀｰｰｰ(ﾟAﾟ)ｰｰｰ!!!\n", cmd_buf);
		}
	}

	/* 入力を促す */
	putString("> ");
}

/* コマンドバッファーに文字をためてから画面に文字を書く */
void Console::putKey(byte c)
{
	/* タブはスペースに変換しよう */
	if (c == '\t') {
		c = ' ';
	}
	
	/* 改行 */
	if (c == '\n') {
		restoreChar();
		locate(INSETS_LEFT, cy + FONT_H);
		parseCommand();
		memset(cmd_buf, '\0', 128);
		drawCarret();
		return;
	}

	/* バックスペース*/
	if (c == '\b' && strlen(cmd_buf) > 0) {
		cmd_buf[strlen(cmd_buf) - 1] = '\0';
		cx -= FONT_W;
	}

	/* 半角英数字 */
	else if (0x20 <= c && c < 0x7f) {
		cmd_buf[strlen(cmd_buf)] = c;
	}

	restoreChar();
	putChar(c);
	drawCarret();
}

/* 1文字分の背景を復元する */
void Console::restoreChar()
{
	for (int y = 0; y < FONT_H; y++) {
		for (int x = 0; x < FONT_W * 2; x++) {
			putPixel(cx + x, cy + y, cb);
		}
	}
	
	/* 部分再描画 */
	SheetCtl::refresh(
		sheet, 
		cx, 
		cy, 
		cx + (FONT_W * 2) + 1, 
		cy + FONT_H + 1
	);
}

/* 画面にキャレットを書く */
void Console::drawCarret()
{
	for (int x = 0; x < FONT_W; x++) {
		putPixel(cx + x, cy + FONT_H - 2, cc);
		putPixel(cx + x, cy + FONT_H - 1, cc);
	}
	
	/* 部分再描画 */
	SheetCtl::refresh(
		sheet, 
		cx, 
		cy + FONT_H - 2, 
		cx + FONT_W + 1, 
		cy + FONT_H + 1
	);
}

/* 画面に点を打つ */
void Console::putPixel(int x, int y, word color)
{
	if (x < 0 || y < 0 || x >= cw || y >= ch) return;
	vram[y * cw + x] = color;
}

/* 画面に文字を書く */
void Console::putChar(byte c)
{
	int pos = 0;

	/* 改行 */
	if (c == '\n') {
		locate(INSETS_LEFT, cy + FONT_H);
		return;
	}

	/* タブ */
	else if (c == '\t') {
		putChar(' ');
		putChar(' ');
		putChar(' ');
		putChar(' ');
		return;
	}

	/* 半角英数字 */
	else if (0x20 <= c && c < 0x7f) {
		pos = c - 0x20;
	}

#if 1
	/* 半角カタカナ */
	else if (0xA1 <= c && c <= 0xdf) {
		pos = c - 0xa1 + 95;
	}

	/* それ以外の文字 */
	else {
		return;
	}

	int cnt = 2;
	int bit = 1;

	for (int y = 0; y < FONT_H; y++) {
		for (int x = 0; x < FONT_W; x++) {
			if ((font_hankaku[pos][cnt] & bit) != 0) {
				putPixel(cx + x, cy + y, cc);
			}
			bit <<= 1;
			if (bit == 256) {
				cnt++;
				bit = 1;
			}
		}
	}
#endif
#if 0
	/* それ以外の文字 */
	else {
		return;
	}

	int x, y;
	for (y = 0; y < FONT_H; y++) {
		for (x = 0; x < FONT_W; x++) {
			if ((font_latin1[pos][y] >> (7 - x)) & 0x1) {
				putPixel(cx + x, cy + y, cc);
			}
		}
	}
#endif

	/* 部分再描画 */
	SheetCtl::refresh(
		sheet, 
		cx, 
		cy, 
		cx + FONT_W + 1, 
		cy + FONT_H + 1
	);

	locate(cx + FONT_W, cy);
}

/* 画面に文字列を書く */
void Console::putString(const char* str)
{
	while (*str != '\0') {
		putChar(*str & 0xff);
		str++;
	}
}

/* 画面に書式化された文字列を書く */
void Console::printf(const char* format, ...)
{
	int buf_size = 128;
	memset(prt_buf, 0, buf_size);
	va_list args;
	va_start(args, format);
	int result = vsprintf(prt_buf, format, args);
	va_end(args);
	if(result > buf_size) {
		prt_buf[buf_size - 1] = 0;
	}
	putString(prt_buf);
}
