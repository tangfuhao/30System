#include "bootpack.h"

void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf){
	fifo->size = size;
	fifo->buf = buf;
	fifo->freedata = size;
	fifo->flags = 0;
	fifo->p = 0;/*下一个数据写入位置*/
	fifo->q = 0;/*下一个数据读取位置*/

	return ;
}

int fifo8_put(struct FIFO8 *fifo, unsigned char data){
	if (fifo->freedata == 0)
	{
		fifo->flags |= FLAGS_OVERRUN;
		return -1;
	}

	fifo->buf[fifo->p] = data;
	fifo->p++;
	if (fifo->p == fifo->size)
	{
		fifo->p = 0;
	}
	fifo->freedata --;

	return 0;
}

int fifo8_get(struct FIFO8 *fifo){
	if (fifo->freedata == fifo->size) return -1;
	int data = fifo->buf[fifo->q];
	fifo->q ++;
	if (fifo->q == fifo->size)
	{
		fifo->q = 0;
	}
	fifo->freedata ++;

	return data;
}

int fifo8_status(struct FIFO8 *fifo){
	return fifo->size - fifo->freedata;
}

