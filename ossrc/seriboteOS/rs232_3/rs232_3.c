#include "apilib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void RS232_putchar16(char *data, int index);
void RS232_putstr4(char *str);
void RS232_close();
void RS232_waitequ();
int rs232_open_status = -1;
int rs232_close_status = -1;
int st;
char s[32];
char hankaku_bin[4096];
void HariMain(void)
{
	char c, c0;
	int fileHandle;
	int index = 0x03; /* LEDのどこに表示するか
					  左から 0x03, 0x04, 0x05, 0x06*/
	int i;
	char cmdline[32];
	api_putstr0("RS232 コントロールプログラム\n");
	api_putstr0("by uchan\n");
	api_putstr0("COM1ポートを開いています\n");
	rs232_open_status = api_rs232_open("COM1");
	if(rs232_open_status != 0)
	{
		sprintf(s, "status = %d\n", rs232_open_status);
		api_putstr0(s);
		api_putstr0("COM1ポート    [FAILED]\n");
		api_putstr0("終了します。\n");
	}
	else{
		api_putstr0("COM1ポート    [  OK  ]\n");
		api_cmdline(cmdline, 32);
		for(i = 0; cmdline[i] > ' '; i++);
		for(; cmdline[i] == ' '; i++);
		//fileHandle = api_fopen("hankaku.bin");
		fileHandle = api_fopen(cmdline + i);
		if(fileHandle == 0)
		{
			RS232_close();
			api_end();
		}
		/* hankaku.binが開けた */
		if(api_fread(hankaku_bin, 4096, fileHandle) != 4096)
		{
			api_putstr0("4096バイトではなかった");
			RS232_close();
			api_end();
		}
		api_putstr0("LEDの電源を入れ直してください\n");
		api_putstr0("何かキーを押してください\n");
		api_getkey(1);
		api_rs232_putchar("COM1", 0x10); /* スクロール停止 */
		RS232_waitequ();
		api_rs232_putchar("COM1", 0x00); /* 表示クリア */
		RS232_waitequ();
		RS232_putstr4("BOOT");
		api_rs232_putchar("COM1", 0x11); /* スクロール再開 */
		RS232_waitequ();
		for(;;)
		{
			api_putstr0("input >");
			i = 0;
			do
			{
				c = api_getkey(1) & 0xff;
				s[i++] = c;
				api_putchar(c);
			} while(c != '\n' && i < 4);
			if(i == 1 && c == '\n')
			{
				break;
			}
			s[i] = '\0';
			api_putchar('\n');
			api_rs232_putchar("COM1", 0x10); /* スクロール停止 */
			RS232_waitequ();
			api_rs232_putchar("COM1", 0x00); /* 表示クリア */
			RS232_waitequ();
			RS232_putstr4(s);
			api_rs232_putchar("COM1", 0x11); /* スクロール再開 */
			RS232_waitequ();
			//api_putchar(c);
/*			if(index >= 0x07)
			{
				index = 0x03;
			}
*/			//RS232_putchar16(hankaku_bin + c * 16, index);
			//sprintf(s, "%cee", c);
			//RS232_putstr4("test");
//			index++;
			//api_putstr0("\'=\'の受信待ち\n");
/*			while(st != '=')
			{
				st = api_rs232_getchar("COM1");
			}
*/		}
		RS232_close();
	}
	api_end();
}

void RS232_putchar16(char *data, int index)
{
	int i;
	api_rs232_putchar("COM1", index);
	RS232_waitequ();
	for(i = 0; i < 16; i++)
	{
		api_rs232_putchar("COM1", data[i]);
		RS232_waitequ();
	}
	return;
}

void RS232_putstr4(char *str)
{
	int i;
	for(i = 0; i < 4 && str[i] != '\0'; i++)
	{
		RS232_putchar16(str[i]*16 + hankaku_bin, i+0x03);
	}
}

void RS232_close()
{
	api_putstr0("COM1ポートを閉じています\n");
	rs232_close_status = api_rs232_close("COM1");
	if(rs232_open_status != 0)
	{
		sprintf(s, "status = %d\n", rs232_close_status);
		api_putstr0(s);
		api_putstr0("COM1ポート    [FAILED]\n");
		api_putstr0("終了します。\n");
	}
	else
	{
		api_putstr0("COM1ポート    [  OK  ]\n");
	}
}

void RS232_waitequ()
{
	//api_putstr0("\'=\'の受信待ち\n");
	st = api_rs232_getchar("COM1");
	while(st != '=')
	{
		st = api_rs232_getchar("COM1");
		if(api_getkey(0) == 'q')
			break;
	}
}
