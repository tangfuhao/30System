#include "bayos.h"

/* C言語の関数宣言 */
extern "C" {
	/* C言語の入り口 */
	void HariMain(void);
};

/* メモリマネージャ */
Memory g_km;

/* C言語の入り口 */
void HariMain(void)
{
	dword size = get_physical_size(0x00400000, 0xbfffffff); /* 物理メモリサイズ */
	g_km.init(0x00400000, size); /* メモリマネージャ初期化 */

	Descriptor::init();       /* GDT/IDTの初期化 */
	PIC::init();              /* PICの初期化 */
	PIT::init();              /* PITの初期化 */
	File::init();             /* ファイルマネージャの初期化 */
	//Screen::init();           /* 画面の初期化 */
	SheetCtl::init();         /* 下敷きの初期化 */
	Console::init();          /* コンソールを初期化 */
	Keyboard::init();         /* キーボードの初期化 */
	Mouse::init();            /* マウスの初期化 */
	Scheduler::init();        /* スケジューラの初期化 */
	io_out8(PIC0_IMR, 0xf8);  /* PITとPIC1とキーボードを許可(11111000) */
	io_out8(PIC1_IMR, 0xed);  /* ネットワークとマウスを許可(11101101) */
	
	#if 0
	Sheet* s1 = new Sheet(10, 10, 300, 200, -1 /* 不透過 */);
	memset(s1->buffer, 7, 300 * 200);
	Sheet::add(s1);
	//s1->locate(20, 20);

	Sheet* s2 = new Sheet(20, 20, 300, 200, -1 /* 不透過 */);
	memset(s2->buffer, 4, 300 * 200);
	Sheet::add(s2);
	//s2->locate(30, 30);

	Sheet* s3 = new Sheet(30, 30, 300, 200, -1 /* 不透過 */);
	memset(s3->buffer, 1, 300 * 200);
	Sheet::add(s3);
	//s3->locate(40, 40);
	#endif
	
	io_sti();                 /* IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除 */
	
	for (;;) {
		io_stihlt();          /* hlt しないと CPU 100% になる */
	}
}
