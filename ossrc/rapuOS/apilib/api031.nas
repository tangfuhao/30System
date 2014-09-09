[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api031.nas"]

		GLOBAL	_api_bitbltEx

[SECTION .text]

_api_bitbltEx:	; int api_bitbltEx(int win, short *buf, int x, int y, int sx, int sy, int xsize, int ysize, int width);
		PUSH	EDI
		PUSH	ESI
		PUSH	EBP
		PUSH	EBX
		MOV		EDX,31
		MOV		EBX,[ESP+20]	; win
		MOV		EAX,[ESP+24]	; buf
		MOV		ECX,[ESP+28]	; x
		SHL		ECX,16
		ADD		ECX,[ESP+32]	; y
		MOV		ESI,[ESP+36]	; sx
		SHL		ESI,16
		ADD		ESI,[ESP+40]	; sy
		MOV		EDI,[ESP+44]	; xsize
		SHL		EDI,16
		ADD		EDI,[ESP+48]	; ysize
		MOV		EBP,[ESP+52]	; width
		INT		0x40
		POP		EBX
		POP		EBP
		POP		ESI
		POP		EDI
		RET
