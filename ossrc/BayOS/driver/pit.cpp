#include "../bootpack.h"

dword PIT::count = 0;

/* タイマーの初期化 */
void PIT::init()
{
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, 0x9c); /* 10ms */
	io_out8(PIT_CNT0, 0x2e);
}

/* 指定した時間だけ待つ */
void PIT::sleep(dword ms)
{
	dword wakeup = PIT::count + (ms / 10);
	while (PIT::count < wakeup) {
		io_hlt();
	}
}
