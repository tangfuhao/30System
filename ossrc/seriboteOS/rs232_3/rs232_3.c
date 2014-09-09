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
	int index = 0x03; /* LED�̂ǂ��ɕ\�����邩
					  ������ 0x03, 0x04, 0x05, 0x06*/
	int i;
	char cmdline[32];
	api_putstr0("RS232 �R���g���[���v���O����\n");
	api_putstr0("by uchan\n");
	api_putstr0("COM1�|�[�g���J���Ă��܂�\n");
	rs232_open_status = api_rs232_open("COM1");
	if(rs232_open_status != 0)
	{
		sprintf(s, "status = %d\n", rs232_open_status);
		api_putstr0(s);
		api_putstr0("COM1�|�[�g    [FAILED]\n");
		api_putstr0("�I�����܂��B\n");
	}
	else{
		api_putstr0("COM1�|�[�g    [  OK  ]\n");
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
		/* hankaku.bin���J���� */
		if(api_fread(hankaku_bin, 4096, fileHandle) != 4096)
		{
			api_putstr0("4096�o�C�g�ł͂Ȃ�����");
			RS232_close();
			api_end();
		}
		api_putstr0("LED�̓d������꒼���Ă�������\n");
		api_putstr0("�����L�[�������Ă�������\n");
		api_getkey(1);
		api_rs232_putchar("COM1", 0x10); /* �X�N���[����~ */
		RS232_waitequ();
		api_rs232_putchar("COM1", 0x00); /* �\���N���A */
		RS232_waitequ();
		RS232_putstr4("BOOT");
		api_rs232_putchar("COM1", 0x11); /* �X�N���[���ĊJ */
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
			api_rs232_putchar("COM1", 0x10); /* �X�N���[����~ */
			RS232_waitequ();
			api_rs232_putchar("COM1", 0x00); /* �\���N���A */
			RS232_waitequ();
			RS232_putstr4(s);
			api_rs232_putchar("COM1", 0x11); /* �X�N���[���ĊJ */
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
			//api_putstr0("\'=\'�̎�M�҂�\n");
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
	api_putstr0("COM1�|�[�g����Ă��܂�\n");
	rs232_close_status = api_rs232_close("COM1");
	if(rs232_open_status != 0)
	{
		sprintf(s, "status = %d\n", rs232_close_status);
		api_putstr0(s);
		api_putstr0("COM1�|�[�g    [FAILED]\n");
		api_putstr0("�I�����܂��B\n");
	}
	else
	{
		api_putstr0("COM1�|�[�g    [  OK  ]\n");
	}
}

void RS232_waitequ()
{
	//api_putstr0("\'=\'�̎�M�҂�\n");
	st = api_rs232_getchar("COM1");
	while(st != '=')
	{
		st = api_rs232_getchar("COM1");
		if(api_getkey(0) == 'q')
			break;
	}
}
