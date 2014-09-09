[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api031.nas"]

		GLOBAL	_api_rs232_getchar

[SECTION .text]

_api_rs232_getchar:		; int api_rs232_getchar(char *comPortName);
		MOV		EDX,31
		MOV		EAX,[ESP+4]		; comPortName
		INT		0x40
		RET
