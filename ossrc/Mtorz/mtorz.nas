[FORMAT "BIN"]
[INSTRSET "i386"]
[OPTIMIZE 1]
[OPTION 1]
[BITS 16]

	ORG	0x0100 ; .COM�͕K��ORG 0x0100�Ŏn�܂�

	MOV	AX,0x0003
	INT	0x10

	MOV	AH,0x01
	MOV	CH,0x20
	INT	0x10

	MOV	byte [col],7
	MOV	CX,45
	MOV	DL,2
	MOV	DH,0
	MOV     BP,boot
	CALL    putstring
	MOV	byte [col],0

mainlop:
	MOV	CX,3
	MOV	DL,39
	MOV	DH,11
	MOV     BP,orz1
	CALL    putstring

	MOV	CX,7
	MOV	DL,37
	MOV	DH,12
	MOV     BP,orz2
	CALL    putstring

	MOV	CX,11
	MOV	DL,35
	MOV	DH,13
	MOV     BP,orz3
	CALL    putstring

	MOV	CX,15
	MOV	DL,33
	MOV	DH,14
	MOV     BP,orz4
	CALL    putstring

	CALL	wait

	INC	byte [col]
	CMP	byte [col],8
	JBE	mainlop

	MOV	byte [col],0
	JMP	mainlop

	JMP	$

putstring:
	mov	BL,[col]
	MOV	AX,0x1300
	MOV	BH,0
	INT	0x10
	RET

wait:
	MOV	AH,0x00		;�N���b�N�J�E���g�ǂݏo��
	INT	0x1a		;���s�I
	MOV	[time0],DX	;DX��time0�ɂ����
wait_loop:
	MOV	AH,0x00		;�������N���b�N�J�E���g�ǂݏo��
	INT	0x1a		;���s�I
	SUB	DX,[time0]	;DX-=time0
	CMP	DX,9		;DX��9���r
	JB	wait_loop	;9��菬���������烋�[�v
	RET

boot	DB      "Mt.orz   Ver.0.2     (C) 2004-2006 Akkiesoft.",0
orz1	DB      "orz",0
orz2	DB      "orz orz",0
orz3	DB      "orz orz orz",0
orz4	DB      "orz orz orz orz",0

col		dw	0
time0	dw	0
