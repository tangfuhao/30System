/* RS232の制御
written by uchan
*/
#include "bootpack.h"
#include "string.h"

#define NULL 0

void rs232_cli(void); /* rs232のIRQ割り込みを無効にする */
void rs232_sti(void); /* rs232のIRQ割り込みを有効にする */
void rs232_baudrate(struct ComPort *COM, int baudrate);

struct ComPort COM1, COM2;

void inthandler23(int *esp)
/* IRQ-3 : COM2 */
{
	io_out8(PIC0_OCW2, 0x63);	/* IRQ-03受付完了をPIC0に通知 */
	return;
}

void inthandler24(int *esp)
/* IRQ-4 : COM1 */
{
	char intID;
	char intTrig; /* どの割り込み要因で割り込んだのか */
	/* まずインタラプトIDを調べる */
	intID = io_in8(0x3fa);

	if((intID & 0x01) != 0)/* 割り込み無しだった */
	{
		io_out8(PIC0_OCW2, 0x64);	/* IRQ-04受付完了をPIC0に通知 */
		return;
	}

	intTrig = (intID >> 1) & 0x07;
	if((intID & 0xc0) == 0x00)
	/* 16450互換モードである */
	{
		switch(intTrig)/* 優先度が高いほど上に書いてある */
		{
		case 0x03: /* Rx line status */
			break;
		case 0x02: /* Rx data */
			fifo32_put(&COM1.RxData, (int)io_in8(0x3f8));
			break;
		case 0x01: /* Tx data empty */
			if(fifo32_status(&COM1.TxData) > 0) /* 送信データが空じゃなかったら送信 */
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
	/* 16550互換モードである */
	{
		switch(intTrig)/* 優先度が高いほど上に書いてある */
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
	io_out8(PIC0_OCW2, 0x64);	/* IRQ-04受付完了をPIC0に通知 */
	return;
}

void init_rs232(void)
{
	/* 16550互換モードと16450互換モードがあるが、今回は16450互換モードを使う
	だから、FIFOコントロールレジスタ(0x03FA)のbit0を0にする。*/

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
	COM2.baudrate = 0; /* いったん0 */
	COM2.IRQ_enable = 0x03; /* IRQは、受信データ割り込みと、送信データ空割り込み */
	COM2.line_ctrl = 0x03; /* 8bit長 */
	COM2.modem_ctrl = 0x00; /* 制御PIN全無効化 */
	COM2.task = NULL;
	/*
	RS232の初期化。
	*/
	rs232_cli();

//	io_out8(0x3fb, 0x80);
//	io_out8(0x3f8, 0x0c);
	io_out8(0x3fb, COM1.line_ctrl); /* 8bit長 */
//	io_out8(0x3fb, 0x03);
	io_out8(0x3fc, COM1.modem_ctrl); /* 制御PIN全無効化 */
//	io_out8(0x3fc, 0x00);
	io_out8(0x3f9, COM1.IRQ_enable); /* IRQは、受信データ割り込みのみ */
//	io_out8(0x3f9, 0x00);
	rs232_baudrate(&COM1, 9600);

	io_out8(0x2fb, COM2.line_ctrl); /* 8bit長 */
	io_out8(0x2fc, COM2.modem_ctrl); /* 制御PIN全無効化 */
	io_out8(0x2f9, COM2.IRQ_enable); /* IRQは、受信データ割り込みのみ */
	rs232_baudrate(&COM2, 9600);

	io_in8(0x3f8); /* 空読み */
	io_in8(0x2f8); /* 空読み */

	rs232_sti();
	/*
	初期設定完了。
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
/* rs232のIRQ割り込みを無効にする */
{
	char status;
	status = io_in8(PIC0_IMR);
	status |= 0x18; /* COM1,2のIRQ禁止 */
	io_out8(PIC0_IMR, status);
	return;
}
void rs232_sti(void)
/* rs232のIRQ割り込みを有効にする */
{
	char status;
	status = io_in8(PIC0_IMR);
	status &= 0xe7; /* COM1,2のIRQ許可 */
	io_out8(PIC0_IMR, status);
	return;
}

void rs232_baudrate(struct ComPort *COM, int baudrate)
/*
ボーレート設定関数
引数
	baudrate : 設定したいボーレート値(ex.9600)
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
		io_out8(0x3fb, (line_control | 0x80)); /* ボーレート設定を可能にする */
		io_out8(0x3f8, LSB); /* ボーレートのLSB */
		io_out8(0x3f9, MSB); /* ボーレートのMSB(LSBと併せて9600bps) */
		io_out8(0x3fb, line_control); /* 設定を元に戻す */
		COM->baudrate = baudrate;
	} else if(strcmp(COM->PortName, "COM2") == 0)
	{
		line_control = io_in8(0x02fb);
		io_out8(0x2fb, (line_control | 0x80)); /* ボーレート設定を可能にする */
		io_out8(0x2f8, LSB); /* ボーレートのLSB */
		io_out8(0x2f9, MSB); /* ボーレートのMSB(LSBと併せて9600bps) */
		io_out8(0x2fb, line_control); /* 設定を元に戻す */
		COM->baudrate = baudrate;
	}
}

int rs232_put(struct ComPort *COM, char data, struct TASK *task)
{
	static int first = 1;

	if(COM->task != task){ return -3; /* 不正終了 */ }

	if(first == 1)/* 初めてのデータ送信 */
	{
		first = 0;
		if(strcmp(COM->PortName, "COM1") == 0)
		{
			io_out8(0x3f8, data);
		} else if(strcmp(COM->PortName, "COM2") == 0)
		{
			io_out8(0x2f8, data);
		}
		return 0; /* 正常終了 */
	}
	io_cli();
	if((io_in8(0x3fd) & 0x40) == 0x00 && first == 0)
	/* データの送信が完了していない。つまり、送信完了割り込みが発生する。 */
	{
		fifo32_put(&COM->TxData, data);
		io_sti(); /* データをFIFOに入れる前に割り込まれると、そのデータは送信されず残ってしまう。 */
	} else if((io_in8(0x3fd) & 0x40) == 0x40 && first == 0)
	/* データの送信が完了している。つまり、即座に送信 */
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
	return 0; /* 正常終了 */
}

int rs232_get(struct ComPort *COM, struct TASK *task)
{
	if(COM->task != task){ return -3; /* 不正終了 */ }
	return fifo32_get(&COM->RxData);
//	return 'b';
}

signed int open_rs232(struct ComPort *COM, struct TASK *task)
{
	if((int)COM->task == NULL)
	{
		COM->task = task;
		return 0; /* 正常終了 */
	}
	else
	{
		return -1; /* 不正終了 */
	}
	return -3;
}

int close_rs232(struct ComPort *COM, struct TASK *task)
{
	if(COM->task == task)
	{
		COM->task = NULL;
		return 0; /* 正常終了 */
	}
	else
	{
		return -1; /* 不正終了 */
	}
	return -3;
}



