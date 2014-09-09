[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api029.nas"]

		GLOBAL	_api_rs232_close

[SECTION .text]

_api_rs232_close:		; int api_rs232_close(char *comPortName);
		MOV		EDX,29
		MOV		EAX,[ESP+4]		; comPortName
		INT		0x40
		RET
