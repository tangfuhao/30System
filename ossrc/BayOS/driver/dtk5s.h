#if !defined(_BAYOS_UTIL_TEK5SDECODER_H_INCLUDED_)
#define _BAYOS_UTIL_TEK5SDECODER_H_INCLUDED_

int tek_checkformat(int siz, byte *p); /* �W�J��̃T�C�Y��Ԃ� */
/* -1:��osacmp */
/* -2:osacmp�����Ή��ł��Ȃ� */

int tek_decode(int siz, byte *p, byte *q); /* ����������0 */
/* ���̒l�̓t�H�[�}�b�g�ُ̈�E���Ή��A���̒l�̓������s�� */

#endif
