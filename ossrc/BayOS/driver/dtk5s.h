#if !defined(_BAYOS_UTIL_TEK5SDECODER_H_INCLUDED_)
#define _BAYOS_UTIL_TEK5SDECODER_H_INCLUDED_

int tek_checkformat(int siz, byte *p); /* 展開後のサイズを返す */
/* -1:非osacmp */
/* -2:osacmpだが対応できない */

int tek_decode(int siz, byte *p, byte *q); /* 成功したら0 */
/* 正の値はフォーマットの異常・未対応、負の値はメモリ不足 */

#endif
