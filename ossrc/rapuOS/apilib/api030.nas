[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api030.nas"]

		GLOBAL	_api_autorefresh

[SECTION .text]

_api_autorefresh:		; void api_autorefresh(int win, int sw);
		PUSH	EBX
		MOV		EDX,30
		MOV		EBX,[ESP+ 8]		; win
		MOV		EAX,[ESP+12]		; sw
		INT		0x40
		POP		EBX
		RET
