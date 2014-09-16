/* bootpack‚ÌƒƒCƒ“ */

#include "bootpack.h"
#include <stdio.h>



struct FIFO8 keyfifo;
struct FIFO8 mousefifo;
struct MOUSE_DEC mdec;
void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256],keybuf[32],mousebuf[128];
	int mx, my, i;

	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC‚Ì‰Šú‰»‚ªI‚í‚Á‚½‚Ì‚ÅCPU‚ÌŠ„‚èž‚Ý‹ÖŽ~‚ð‰ðœ */

	fifo8_init(&keyfifo,32,keybuf);
	fifo8_init(&mousefifo,128,mousebuf);
	io_out8(PIC0_IMR, 0xf9); /* PIC1‚ÆƒL[ƒ{[ƒh‚ð‹–‰Â(11111001) */
	io_out8(PIC1_IMR, 0xef); /* ƒ}ƒEƒX‚ð‹–‰Â(11101111) */

	init_keyboard();

	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2; /* ‰æ–Ê’†‰›‚É‚È‚é‚æ‚¤‚ÉÀ•WŒvŽZ */
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	
	enable_mouse(&mdec);


	
	for (;;) {
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo)== 0){
			io_stihlt();
		}else{
			if (fifo8_status(&keyfifo) != 0)
			{
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s,"%02X",i);
				putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
			}else if(fifo8_status(&mousefifo) != 0){
				i = fifo8_get(&mousefifo);
				io_sti();

				if (mouse_decode(&mdec,i) == 1)
				{

					sprintf(s,"[lcr %4d %4d]",mdec.x,mdec.y);
					if ((mdec.btn & 0x01) != 0)
					{
						s[1] = 'L';
					}
					if ((mdec.btn & 0x02) != 0)
					{
						s[3] = 'R';
					}
					if ((mdec.btn & 0x04) != 0)
					{
						s[2] = 'C';
					}

					putfonts8_asc(binfo->vram, binfo->scrnx, 32, 32, COL8_FFFFFF, s);


					/* 鼠标移动 */
					boxfill8(binfo->vram,binfo->scrnx,COL8_008484,mx,my,mx+15,my+15);/* 隐藏鼠标 */
					mx += mdec.x;
					my += mdec.y;
					if (mx<0){
						mx = 0;
					}
					if (my<0){
						my = 0;
					}
					if (mx > binfo->scrnx - 16){
						mx = binfo->scrnx - 16;
					}
					if (my > binfo->scrny - 16){
						my = binfo->scrny - 16;
					}
					sprintf(s,"(%3d,%3d)",mx,my);
					putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
					putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);
				}

				
			}
			
		}
	}
}

void wait_KBC_sendready(void)
{
	int i;
	for (i = 0; i < 1000000; i++) {
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) break;
	}
}

void init_keyboard(void)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

void enable_mouse(struct MOUSE_DEC *mdec)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);

	mdec->phase = 0;/* 等待0xfa 阶段 */
	return; 
}

int mouse_decode(struct MOUSE_DEC *mdec,unsigned char dat){
	if (mdec->phase == 0){
		if (dat == 0xfa)
		{
			mdec->phase = 1;
		}
		return 0;
	}else if (mdec->phase == 1){
		/* 等待鼠标第一个阶段*/
		if ((dat & 0xc8) == 0x08){
			mdec->buf[0] = dat;
			mdec->phase = 2;
		}
		return 0;
	}else if (mdec->phase == 2){
		mdec->buf[1] = dat;
		mdec->phase = 3;
		return 0;
	}else if (mdec->phase == 3){
		mdec->buf[2] = dat;
		mdec->phase = 1;

		mdec->btn = mdec->buf[0] & 0x07;
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];
		if ((mdec->buf[0] & 0x10) != 0)
		{
			mdec->x |= 0xffffff00;
		}
		if ((mdec->buf[1] & 0x10) != 0)
		{
			mdec->y |= 0xffffff00;
		}
		mdec->y = -mdec->y;

		if ((mdec->x - mdec->last_x) < 50 
			&& (mdec->x - mdec->last_x) > -50 
			&& (mdec->y - mdec->last_y) < 50 
			&& (mdec->y - mdec->last_y) > -50)
		{
			mdec->last_x = mdec->x;
			mdec->last_y = mdec->y;
			return 1;
		}else{
			mdec->phase = 1;
			return 0;
		}
	}
	return -1;
}


