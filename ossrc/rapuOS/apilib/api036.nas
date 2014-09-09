[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "api036.nas"]

		GLOBAL	_api_sysinfo

[SECTION .text]

_api_sysinfo:	; void api_sysinfo(struct SYSINFO *sysinfo);
		MOV		EDX,36
		MOV		EAX,[ESP+4]			; sysinfo
		INT		0x40
		RET
