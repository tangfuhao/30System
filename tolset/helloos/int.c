/* Š„‚èž‚ÝŠÖŒW */

#include "bootpack.h"
#include <stdio.h>

#define PORT_KEYDAT		0x0060
struct KEYBUF keybuf ;

void init_pic(void)
/* PIC‚Ì‰Šú‰» */
{
	io_out8(PIC0_IMR,  0xff  ); /* ‘S‚Ä‚ÌŠ„‚èž‚Ý‚ðŽó‚¯•t‚¯‚È‚¢ */
	io_out8(PIC1_IMR,  0xff  ); /* ‘S‚Ä‚ÌŠ„‚èž‚Ý‚ðŽó‚¯•t‚¯‚È‚¢ */

	io_out8(PIC0_ICW1, 0x11  ); /* ƒGƒbƒWƒgƒŠƒKƒ‚[ƒh */
	io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7‚ÍAINT20-27‚ÅŽó‚¯‚é */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1‚ÍIRQ2‚É‚ÄÚ‘± */
	io_out8(PIC0_ICW4, 0x01  ); /* ƒmƒ“ƒoƒbƒtƒ@ƒ‚[ƒh */

	io_out8(PIC1_ICW1, 0x11  ); /* ƒGƒbƒWƒgƒŠƒKƒ‚[ƒh */
	io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15‚ÍAINT28-2f‚ÅŽó‚¯‚é */
	io_out8(PIC1_ICW3, 2     ); /* PIC1‚ÍIRQ2‚É‚ÄÚ‘± */
	io_out8(PIC1_ICW4, 0x01  ); /* ƒmƒ“ƒoƒbƒtƒ@ƒ‚[ƒh */

	io_out8(PIC0_IMR,  0xfb  ); /* 11111011 PIC1ˆÈŠO‚Í‘S‚Ä‹ÖŽ~ */
	io_out8(PIC1_IMR,  0xff  ); /* 11111111 ‘S‚Ä‚ÌŠ„‚èž‚Ý‚ðŽó‚¯•t‚¯‚È‚¢ */

	return;
}

void inthandler21(int *esp)
/* PS/2ƒL[ƒ{[ƒh‚©‚ç‚ÌŠ„‚èž‚Ý */
{
	unsigned char data;
	io_out8(PIC0_OCW2,0x61);	/* 通知IPC*IRQ-01 已经受理完毕 */
	data = io_in8(PORT_KEYDAT);

	if (keybuf.len < 32)
	{
		keybuf.data[keybuf.next_w] = data;
		keybuf.next_w ++;
		keybuf.len ++;
		if (keybuf.next_w == 32)
		{
			keybuf.next_w = 0;
		}
	}
	
	return ;
}

void inthandler2c(int *esp)
/* PS/2ƒ}ƒEƒX‚©‚ç‚ÌŠ„‚èž‚Ý */
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");

	return ;
}

void inthandler27(int *esp)
/* PIC0‚©‚ç‚Ì•sŠ®‘SŠ„‚èž‚Ý‘Îô */
/* Athlon64X2‹@‚È‚Ç‚Å‚Íƒ`ƒbƒvƒZƒbƒg‚Ì“s‡‚É‚æ‚èPIC‚Ì‰Šú‰»Žž‚É‚±‚ÌŠ„‚èž‚Ý‚ª1“x‚¾‚¯‚¨‚±‚é */
/* ‚±‚ÌŠ„‚èž‚Ýˆ—ŠÖ”‚ÍA‚»‚ÌŠ„‚èž‚Ý‚É‘Î‚µ‚Ä‰½‚à‚µ‚È‚¢‚Å‚â‚è‰ß‚²‚· */
/* ‚È‚º‰½‚à‚µ‚È‚­‚Ä‚¢‚¢‚ÌH
	¨  ‚±‚ÌŠ„‚èž‚Ý‚ÍPIC‰Šú‰»Žž‚Ì“d‹C“I‚ÈƒmƒCƒY‚É‚æ‚Á‚Ä”­¶‚µ‚½‚à‚Ì‚È‚Ì‚ÅA
		‚Ü‚¶‚ß‚É‰½‚©ˆ—‚µ‚Ä‚â‚é•K—v‚ª‚È‚¢B									*/
{
	io_out8(PIC0_OCW2, 0x67); /* IRQ-07Žó•tŠ®—¹‚ðPIC‚É’Ê’m(7-1ŽQÆ) */
	return;
}
