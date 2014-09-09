#include "apilib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void HariMain(void)
{
	char c, c0;
	char s[32];
	int cmd;
	int rs232_open_status = -1;
	int rs232_close_status = -1;
	int st = 0;
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
		for(;;)
		{
			api_putstr0("input >");
			c = api_getkey(1) & 0xff;
			api_putchar('\n');
			api_putchar(c);
			if(c == '0')
			{
				c0 = c;
				c = api_getkey(1) & 0xff;
				if(c == 'x' || c == 'X')
				{
					s[0] = '0';
					s[1] = 'x';
					s[2] = api_getkey(1) & 0xff;
					s[3] = api_getkey(1) & 0xff;
					s[4] = '\0';
					api_putstr0("�R�}���h���[�h���I������܂����B\n");
					api_putstr0(s+1);
					api_putchar('\n');
					cmd = strtol((const char *)s, 0, 16);/* ���͂��ꂽ������𐔎��ɕϊ� */
					api_rs232_putchar("COM1", (char)cmd); /* �R�}���h��0x80�`��������Ă��� */
				}
				else if(c == 'q' || c == 'Q')
				{
					break;
				}
				else
				{
					api_rs232_putchar("COM1", c0);
					api_rs232_putchar("COM1", c);
					api_putchar(c);
				}
			}
			else
			{
				api_rs232_putchar("COM1", c);
			}
			api_putstr0("\'=\'�̎�M�҂�\n");
			while(st != '=')
			{
				st = api_rs232_getchar("COM1");
			}
		}
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
	api_end();
}
