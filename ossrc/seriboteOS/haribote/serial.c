/* RS232�̐���
written by uchan
*/
#include "bootpack.h"
#include "string.h"

#define NULL 0

void rs232_cli(void); /* rs232��IRQ���荞�݂𖳌��ɂ��� */
void rs232_sti(void); /* rs232��IRQ���荞�݂�L���ɂ��� */
void rs232_baudrate(struct ComPort *COM, int baudrate);

struct ComPort COM1, COM2;

void inthandler23(int *esp)
/* IRQ-3 : COM2 */
{
	io_out8(PIC0_OCW2, 0x63);	/* IRQ-03��t������PIC0�ɒʒm */
	return;
}

void inthandler24(int *esp)
/* IRQ-4 : COM1 */
{
	char intID;
	char intTrig; /* �ǂ̊��荞�ݗv���Ŋ��荞�񂾂̂� */
	/* �܂��C���^���v�gID�𒲂ׂ� */
	intID = io_in8(0x3fa);

	if((intID & 0x01) != 0)/* ���荞�ݖ��������� */
	{
		io_out8(PIC0_OCW2, 0x64);	/* IRQ-04��t������PIC0�ɒʒm */
		return;
	}

	intTrig = (intID >> 1) & 0x07;
	if((intID & 0xc0) == 0x00)
	/* 16450�݊����[�h�ł��� */
	{
		switch(intTrig)/* �D��x�������قǏ�ɏ����Ă��� */
		{
		case 0x03: /* Rx line status */
			break;
		case 0x02: /* Rx data */
			fifo32_put(&COM1.RxData, (int)io_in8(0x3f8));
			break;
		case 0x01: /* Tx data empty */
			if(fifo32_status(&COM1.TxData) > 0) /* ���M�f�[�^���󂶂�Ȃ������瑗�M */
			{
				io_out8(0x3f8, fifo32_get(&COM1.TxData));
			}
			break;
		case 0x00: /* modem status */
			break;
		default:
			break;
		}
	} else if((intID & 0xc0) == 0xc0)
	/* 16550�݊����[�h�ł��� */
	{
		switch(intTrig)/* �D��x�������قǏ�ɏ����Ă��� */
		{
		case 0x03: /* Rx line status */
			break;
		case 0x02: /* Rx FIFO trigger  */
			break;
		case 0x06: /* Rx FIFO time-out */
			break;
		case 0x01: /* Tx FIFO empty */
			break;
		case 0x00: /* modem status */
			break;
		default:
			break;
		}
	}
	io_out8(PIC0_OCW2, 0x64);	/* IRQ-04��t������PIC0�ɒʒm */
	return;
}

void init_rs232(void)
{
	/* 16550�݊����[�h��16450�݊����[�h�����邪�A�����16450�݊����[�h���g��
	������AFIFO�R���g���[�����W�X�^(0x03FA)��bit0��0�ɂ���B*/

	strcpy(COM1.PortName, "COM1");
	strcpy(COM2.PortName, "COM2");
	fifo32_init(&(COM1.RxData), 128, COM1.RxDataBuf, 0);
	fifo32_init(&(COM1.TxData), 128, COM1.TxDataBuf, 0);
	fifo32_init(&(COM2.RxData), 128, COM2.RxDataBuf, 0);
	fifo32_init(&(COM2.TxData), 128, COM2.TxDataBuf, 0);
	COM1.baudrate = 0;
	COM1.IRQ_enable = 0x03;
	COM1.line_ctrl = 0x03;
	COM1.modem_ctrl = 0x08;
	COM1.task = NULL;
	COM2.baudrate = 0; /* ��������0 */
	COM2.IRQ_enable = 0x03; /* IRQ�́A��M�f�[�^���荞�݂ƁA���M�f�[�^�󊄂荞�� */
	COM2.line_ctrl = 0x03; /* 8bit�� */
	COM2.modem_ctrl = 0x00; /* ����PIN�S������ */
	COM2.task = NULL;
	/*
	RS232�̏������B
	*/
	rs232_cli();

//	io_out8(0x3fb, 0x80);
//	io_out8(0x3f8, 0x0c);
	io_out8(0x3fb, COM1.line_ctrl); /* 8bit�� */
//	io_out8(0x3fb, 0x03);
	io_out8(0x3fc, COM1.modem_ctrl); /* ����PIN�S������ */
//	io_out8(0x3fc, 0x00);
	io_out8(0x3f9, COM1.IRQ_enable); /* IRQ�́A��M�f�[�^���荞�݂̂� */
//	io_out8(0x3f9, 0x00);
	rs232_baudrate(&COM1, 9600);

	io_out8(0x2fb, COM2.line_ctrl); /* 8bit�� */
	io_out8(0x2fc, COM2.modem_ctrl); /* ����PIN�S������ */
	io_out8(0x2f9, COM2.IRQ_enable); /* IRQ�́A��M�f�[�^���荞�݂̂� */
	rs232_baudrate(&COM2, 9600);

	io_in8(0x3f8); /* ��ǂ� */
	io_in8(0x2f8); /* ��ǂ� */

	rs232_sti();
	/*
	�����ݒ芮���B
	*/
	io_out8(0x3f8, 'c');
	io_out8(0x3f8, 'o');
	io_out8(0x3f8, 'n');
	io_out8(0x3f8, 'e');
	io_out8(0x3f8, 'c');
	io_out8(0x3f8, 't');
	io_out8(0x3f8, 'e');
	io_out8(0x3f8, 'd');
}

void rs232_cli(void)
/* rs232��IRQ���荞�݂𖳌��ɂ��� */
{
	char status;
	status = io_in8(PIC0_IMR);
	status |= 0x18; /* COM1,2��IRQ�֎~ */
	io_out8(PIC0_IMR, status);
	return;
}
void rs232_sti(void)
/* rs232��IRQ���荞�݂�L���ɂ��� */
{
	char status;
	status = io_in8(PIC0_IMR);
	status &= 0xe7; /* COM1,2��IRQ���� */
	io_out8(PIC0_IMR, status);
	return;
}

void rs232_baudrate(struct ComPort *COM, int baudrate)
/*
�{�[���[�g�ݒ�֐�
����
	baudrate : �ݒ肵�����{�[���[�g�l(ex.9600)
*/
{
	char LSB = 0, MSB = 0;
	char line_control;
	switch(baudrate)
	{
	case 115200:
		LSB = 0x01;
		break;
	case 57600:
		LSB = 0x02;
		break;
	case 38400:
		LSB = 0x03;
		break;
	case 19200:
		LSB = 0x06;
		break;
	case 9600:
		LSB = 0x0c;
		break;
	case 4800:
		LSB = 0x18;
		break;
	case 2400:
		LSB = 0x30;
		break;
	case 1200:
		LSB = 0x60;
		break;
	case 600:
		LSB = 0xc0;
		break;
	case 300:
		MSB = 0x01;
		LSB = 0x80;
		break;
	default:
		MSB = 0x00;
		LSB = 0x0c;
	}
	if(strcmp(COM->PortName, "COM1") == 0)
	{
		line_control = io_in8(0x03fb);
		io_out8(0x3fb, (line_control | 0x80)); /* �{�[���[�g�ݒ���\�ɂ��� */
		io_out8(0x3f8, LSB); /* �{�[���[�g��LSB */
		io_out8(0x3f9, MSB); /* �{�[���[�g��MSB(LSB�ƕ�����9600bps) */
		io_out8(0x3fb, line_control); /* �ݒ�����ɖ߂� */
		COM->baudrate = baudrate;
	} else if(strcmp(COM->PortName, "COM2") == 0)
	{
		line_control = io_in8(0x02fb);
		io_out8(0x2fb, (line_control | 0x80)); /* �{�[���[�g�ݒ���\�ɂ��� */
		io_out8(0x2f8, LSB); /* �{�[���[�g��LSB */
		io_out8(0x2f9, MSB); /* �{�[���[�g��MSB(LSB�ƕ�����9600bps) */
		io_out8(0x2fb, line_control); /* �ݒ�����ɖ߂� */
		COM->baudrate = baudrate;
	}
}

int rs232_put(struct ComPort *COM, char data, struct TASK *task)
{
	static int first = 1;

	if(COM->task != task){ return -3; /* �s���I�� */ }

	if(first == 1)/* ���߂Ẵf�[�^���M */
	{
		first = 0;
		if(strcmp(COM->PortName, "COM1") == 0)
		{
			io_out8(0x3f8, data);
		} else if(strcmp(COM->PortName, "COM2") == 0)
		{
			io_out8(0x2f8, data);
		}
		return 0; /* ����I�� */
	}
	io_cli();
	if((io_in8(0x3fd) & 0x40) == 0x00 && first == 0)
	/* �f�[�^�̑��M���������Ă��Ȃ��B�܂�A���M�������荞�݂���������B */
	{
		fifo32_put(&COM->TxData, data);
		io_sti(); /* �f�[�^��FIFO�ɓ����O�Ɋ��荞�܂��ƁA���̃f�[�^�͑��M���ꂸ�c���Ă��܂��B */
	} else if((io_in8(0x3fd) & 0x40) == 0x40 && first == 0)
	/* �f�[�^�̑��M���������Ă���B�܂�A�����ɑ��M */
	{
		io_sti();
		if(strcmp(COM->PortName, "COM1") == 0)
		{
			io_out8(0x3f8, data);
		} else if(strcmp(COM->PortName, "COM2") == 0)
		{
			io_out8(0x2f8, data);
		}
	}
	io_sti();
	return 0; /* ����I�� */
}

int rs232_get(struct ComPort *COM, struct TASK *task)
{
	if(COM->task != task){ return -3; /* �s���I�� */ }
	return fifo32_get(&COM->RxData);
//	return 'b';
}

signed int open_rs232(struct ComPort *COM, struct TASK *task)
{
	if((int)COM->task == NULL)
	{
		COM->task = task;
		return 0; /* ����I�� */
	}
	else
	{
		return -1; /* �s���I�� */
	}
	return -3;
}

int close_rs232(struct ComPort *COM, struct TASK *task)
{
	if(COM->task == task)
	{
		COM->task = NULL;
		return 0; /* ����I�� */
	}
	else
	{
		return -1; /* �s���I�� */
	}
	return -3;
}



