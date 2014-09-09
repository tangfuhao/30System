[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api033.nas"]

		GLOBAL	_api_fopenEx

[SECTION .text]

_api_fopenEx:		; int api_fopenEx(char *fname, int mode);
		PUSH	EBX
		MOV		EDX,33
		MOV		EBX,[ESP+8]			; fname
		MOV		EAX,[ESP+12]		; mode
		INT		0x40
		POP		EBX
		RET
