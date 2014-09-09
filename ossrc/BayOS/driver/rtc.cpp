#include "../bootpack.h"

int RTC::year    = 0;
int RTC::month   = 0;
int RTC::day     = 0;
int RTC::hour    = 0;
int RTC::minutes = 0;
int RTC::second  = 0;

/* ���A���^�C���N���b�N�̏������i������ǂޓx�ɏ��������Ă��������j */
void RTC::init()
{
	char t[7];
	char err;
	static byte adr[] = { 0x00, 0x02, 0x04, 0x07, 0x08, 0x09, 0x32 };
	static byte max[] = { 0x60, 0x59, 0x23, 0x31, 0x12, 0x99, 0x99 };
	for (;;) { /* �ǂݍ��݂���������܂ŌJ��Ԃ� */
		err = 0;
		for (int i = 0; i < 7; i++) {
			io_out8(0x70, adr[i]);
			t[i] = io_in8(0x71);
		}
		for (int i = 0; i < 7; i++) {
			io_out8(0x70, adr[i]);
			if (t[i] != io_in8(0x71) || (t[i] & 0x0f) > 9 || t[i] > max[i]) {
				err = 1;
			}
		}
		if (err == 0) {
			break;
		}
	}
	/* BCS�Ƃ�������Ȍ`����ʏ�̐��l�ɕϊ����� */
	year    = bcd_to_num(t[6]) * 100 + bcd_to_num(t[5]);
	month   = bcd_to_num(t[4]);
	day     = bcd_to_num(t[3]);
	hour    = bcd_to_num(t[2]);
	minutes = bcd_to_num(t[1]);
	second  = bcd_to_num(t[0]);
}
