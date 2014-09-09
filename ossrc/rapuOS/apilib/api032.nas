[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api032.nas"]

		GLOBAL	_api_putstrwinEx

[SECTION .text]

_api_putstrwinEx:	; void api_putstrwinEx(int win, int x, int y, int col, int len, int scl, char *str);
		PUSH	EDI
		PUSH	ESI
		PUSH	EBP
		PUSH	EBX
		MOV		EDX,32
		MOV		EBX,[ESP+20]	; win
		MOV		ESI,[ESP+24]	; x
		MOV		EDI,[ESP+28]	; y
		MOV		EAX,[ESP+32]	; col
		MOV		ECX,[ESP+36]	; len
		SHL		ECX,16
		ADD		ECX,[ESP+40]	; scl
		MOV		EBP,[ESP+44]	; str
		INT		0x40
		POP		EBX
		POP		EBP
		POP		ESI
		POP		EDI
		RET
