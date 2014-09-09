[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api028.nas"]

		GLOBAL	_api_rs232_open

[SECTION .text]

_api_rs232_open:		; int api_rs232_open(char *comPortName);
		MOV		EDX,28
		MOV		EAX,[ESP+4]		; comPortName
		INT		0x40
		RET
