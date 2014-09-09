; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; �I�u�W�F�N�g�t�@�C������郂�[�h	
[INSTRSET "i486p"]				; 486�̖��߂܂Ŏg�������Ƃ����L�q
[BITS 32]						; 32�r�b�g���[�h�p�̋@�B�����点��
[FILE "asm.nas"]			; �\�[�X�t�@�C�������

		GLOBAL	_asm_rdtsc
		GLOBAL	_asm_cpuid

[SECTION .text]

_asm_rdtsc:	; void asm_rdtsc(int *high, int *low);
		PUSHAD
		MOV		EBP,[ESP+36]
		DB		0x0F, 0x31			; RDTSC
		MOV		[EBP],EDX			;
		MOV		[EBP+4],EAX			;
		POPAD
		RET

_asm_cpuid:	; void asm_cpuid(int id_eax, int id_ecx, int *eax, int *ebx, int *ecx, int *edx);
		PUSHAD
		MOV		EAX,[ESP+36]		; id_eax
		MOV		ECX,[ESP+40]		; id_ecx
		MOV		EBP,[ESP+44]
		DB		0x0F, 0xA2			; CPUID
		MOV		[EBP],EAX			;
		MOV		[EBP+4],EBX			;
		MOV		[EBP+8],ECX			;
		MOV		[EBP+12],EDX		;
		POPAD
		RET
