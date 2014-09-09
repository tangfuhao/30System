[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api030.nas"]

		GLOBAL	_api_rs232_putchar

[SECTION .text]

_api_rs232_putchar:		; int api_rs232_putchar(char *comPortName, char data);
		PUSH	ECX
		MOV		EDX,30
		MOV		EAX,[ESP+8]		; comPortName
		MOV		CL,[ESP+12]		; data
		INT		0x40
		POP		ECX
		RET
