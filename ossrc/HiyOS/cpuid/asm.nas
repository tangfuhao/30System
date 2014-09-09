; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; オブジェクトファイルを作るモード	
[INSTRSET "i486p"]				; 486の命令まで使いたいという記述
[BITS 32]						; 32ビットモード用の機械語を作らせる
[FILE "asm.nas"]				; ソースファイル名情報

		GLOBAL	_asm_rdtsc
		GLOBAL	_asm_cpuid

[SECTION .text]

_asm_rdtsc:	; void asm_rdtsc(int *high, int *low);
		PUSHAD
		DB		0x0F, 0x31		; RDTSC
		MOV		EBP,[ESP+36]
		MOV		[EBP],EDX
		MOV		EBP,[ESP+40]
		MOV		[EBP],EAX
		POPAD
		RET

_asm_cpuid:	; void asm_cpuid(int id_eax, int id_ecx, int *eax, int *ebx, int *ecx, int *edx);
		PUSHAD
		MOV		EAX,[ESP+36]
		MOV		ECX,[ESP+40]
		DB		0x0F, 0xA2		; CPUID
		MOV		EBP,[ESP+44]
		MOV		[EBP],EAX	
		MOV		EBP,[ESP+48]
		MOV		[EBP],EBX	
		MOV		EBP,[ESP+52]
		MOV		[EBP],ECX	
		MOV		EBP,[ESP+56]
		MOV		[EBP],EDX	
		POPAD
		RET
