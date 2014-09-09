[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api034.nas"]

		GLOBAL	_api_fcloseEx

[SECTION .text]

_api_fcloseEx:		; void api_fcloseEx(int fhandle);
		MOV		EDX,34
		MOV		EAX,[ESP+4]			; fhandle
		INT		0x40
		RET
