[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api029.nas"]

		GLOBAL	_api_systime

[SECTION .text]

_api_systime:	; void api_systime(char *s);
		PUSH	EBX
		MOV		EDX,29
		MOV		EBX,[ESP+8]		; s
		INT		0x40
		POP		EBX
		RET
