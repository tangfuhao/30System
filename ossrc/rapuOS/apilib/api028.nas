[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL	_api_bitblt

[SECTION .text]

_api_bitblt:		; void api_bitblt(int win, short *buf, int x, int y, int xsize, int ysize);
		PUSH	EDI
		PUSH	ESI
		PUSH	EBP
		PUSH	EBX
		MOV		EDX,28
		MOV		EBX,[ESP+20]	; win
		MOV		EAX,[ESP+24]	; buf
		MOV		ECX,[ESP+28]	; x
		MOV		ESI,[ESP+32]	; y
		MOV		EDI,[ESP+36]	; xsize
		MOV		EBP,[ESP+40]	; ysize
		INT		0x40
		POP		EBX
		POP		EBP
		POP		ESI
		POP		EDI
		RET
