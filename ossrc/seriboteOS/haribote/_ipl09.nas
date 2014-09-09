; haribote-ipl
; TAB=4

CYLS	EQU		9				; どこまで読み込むか

		ORG		0x7c00			; このプログラムがどこに読み込まれるのか

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		; ブートセクタの名前を自由に書いてよい（8バイト）
		DW		512				; 1セクタの大きさ（512にしなければいけない）
		DB		1				; クラスタの大きさ（1セクタにしなければいけない）
		DW		1				; FATがどこから始まるか（普通は1セクタ目からにする）
		DB		2				; FATの個数（2にしなければいけない）
		DW		224				; ルートディレクトリ領域の大きさ（普通は224エントリにする）
		DW		2880			; このドライブの大きさ（2880セクタにしなければいけない）
		DB		0xf0			; メディアのタイプ（0xf0にしなければいけない）
		DW		9				; FAT領域の長さ（9セクタにしなければいけない）
		DW		18				; 1トラックにいくつのセクタがあるか（18にしなければいけない）
		DW		2				; ヘッドの数（2にしなければいけない）
		DD		0				; パーティションを使ってないのでここは必ず0
		DD		2880			; このドライブ大きさをもう一度書く
		DB		0,0,0x29		; よくわからないけどこの値にしておくといいらしい
		DD		0xffffffff		; たぶんボリュームシリアル番号
		DB		"HARIBOTEOS "	; ディスクの名前（11バイト）
		DB		"FAT12   "		; フォーマットの名前（8バイト）
		RESB	18				; とりあえず18バイトあけておく

; プログラム本体

entry:
		MOV		AX,0			; レジスタ初期化
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; ディスクを読む

        MOV     AX,0x0820
        MOV     ES,AX
        MOV     CH,0            ; シリンダ0
        MOV     DH,0            ; ヘッド0
        MOV     CL,2            ; セクタ2
        MOV     BX,18*2-2       ; 読み込みたい合計セクタ数
        CALL    readfast        ; 高速読み込み

        MOV     BX,4312+0x8200+2
fatloop:
        ADD     BX,-2
        CMP     WORD [BX],0
        JZ      fatloop
        MOV     AX,BX
        ADD     AX,-0x8200-5+54+53
        MOV     BL,54
        DIV     BL              ; AL = AX / BL

        CMP     AL,0
        JNZ     fatskip1
        MOV     AL,1
fatskip1:
        CMP     AL,33
        JBE     fatskip2
        MOV     AL,33
fatskip2:
        MOV     [0x0ff0],AL     ; IPLがどこまで読んだのかをメモ
        ADD     AL,-1
        JZ      fatskip3
        MOV     AH,0
        IMUL    BX,AX,18*2

        MOV     AX,0x0c80
        MOV     ES,AX
        MOV     CH,1            ; シリンダ1
        MOV     DH,0            ; ヘッド0
        MOV     CL,1            ; セクタ1
        CALL    readfast        ; 高速読み込み
fatskip3:

; 読み終わったのでharibote.sysを実行だ！

        JMP     0xc200

error:
		MOV		AX,0
		MOV		ES,AX
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; SIに1を足す
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 一文字表示ファンクション
		MOV		BX,15			; カラーコード
		INT		0x10			; ビデオBIOS呼び出し
		JMP		putloop
fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ
msg:
		DB		0x0a, 0x0a		; 改行を2つ
		DB		"load error"
		DB		0x0a			; 改行
		DB		0

readfast:	; ALを使ってできるだけまとめて読み出す
;	ES:読み込み番地, CH:シリンダ, DH:ヘッド, CL:セクタ, BX:読み込みセクタ数

		MOV		AX,ES			; < ESからALの最大値を計算 >
		SHL		AX,3			; AXを32で割って、その結果をAHに入れたことになる
		AND		AH,0x7f			; AHはAHを128で割った余り（512*128=64K）
		MOV		AL,128			; AL = 128 - AH; 一番近い64KB境界まで最大何セクタ入るか
		SUB		AL,AH

		MOV		AH,BL			; < BXからALの最大値をAHに計算 >
		CMP		BH,0			; if (BH != 0) { AH = 18; }
		JE		.skip1
		MOV		AH,18
.skip1:
		CMP		AL,AH			; if (AL > AH) { AL = AH; }
		JBE		.skip2
		MOV		AL,AH
.skip2:

		MOV		AH,19			; < CLからALの最大値をAHに計算 >
		SUB		AH,CL			; AH = 19 - CL;
		CMP		AL,AH			; if (AL > AH) { AL = AH; }
		JBE		.skip3
		MOV		AL,AH
.skip3:

		PUSH	BX
		MOV		SI,0			; 失敗回数を数えるレジスタ
retry:
		MOV		AH,0x02			; AH=0x02 : ディスク読み込み
		MOV		BX,0
		MOV		DL,0x00			; Aドライブ
		PUSH	ES
		PUSH	DX
		PUSH	CX
		PUSH	AX
		INT		0x13			; ディスクBIOS呼び出し
		JNC		next			; エラーがおきなればnextへ
		ADD		SI,1			; SIに1を足す
		CMP		SI,5			; SIと5を比較
		JAE		error			; SI >= 5 だったらerrorへ
		MOV		AH,0x00
		MOV		DL,0x00			; Aドライブ
		INT		0x13			; ドライブのリセット
		POP		AX
		POP		CX
		POP		DX
		POP		ES
		JMP		retry
next:
		POP		AX
		POP		CX
		POP		DX
		POP		BX				; ESの内容をBXで受け取る
		SHR		BX,5			; BXを16バイト単位から512バイト単位へ
		MOV		AH,0
		ADD		BX,AX			; BX += AL;
		SHL		BX,5			; BXを512バイト単位から16バイト単位へ
		MOV		ES,BX			; これで ES += AL * 0x20; になる
		POP		BX
		SUB		BX,AX
		JZ		.ret
		ADD		CL,AL			; CLにALを足す
		CMP		CL,18			; CLと18を比較
		JBE		readfast		; CL <= 18 だったらreadfastへ
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB		readfast		; DH < 2 だったらreadfastへ
		MOV		DH,0
		ADD		CH,1
		JMP		readfast
.ret:
		RET

		RESB	0x7dfe-$		; 0x7dfeまでを0x00で埋める命令

		DB		0x55, 0xaa
