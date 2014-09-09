TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# デフォルト動作

default :
	$(MAKE) rapuos.img

# ファイル生成規則

rapuos.img : haribote/ipl20.bin haribote/haribote.sys Makefile \
		hariapp/a.hrb hariapp/hello3.hrb hariapp/hello4.hrb hariapp/hello5.hrb \
		hariapp/beepdown.hrb hariapp/sosu.hrb hariapp/sosu2.hrb \
		hariapp/type.hrb hariapp/iroha.hrb hariapp/chklang.hrb hariapp/calc.hrb \
		winhelo/winhelo.hrb winhelo2/winhelo2.hrb winhelo3/winhelo3.hrb \
		star1/star1.hrb stars/stars.hrb stars2/stars2.hrb \
		lines/lines.hrb walk/walk.hrb noodle/noodle.hrb \
		color/color.hrb color2/color2.hrb sosu3/sosu3.hrb \
		notrec/notrec.hrb bball/bball.hrb invader/invader.hrb \
		tview/tview.hrb mmlplay/mmlplay.hrb gview/gview.hrb \
		vclock/vclock.hrb cpuid/cpuid.hrb puzzle/puzzle.hrb
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:haribote/ipl20.bin len:512 from:0 to:0 \
		copy from:haribote/haribote.sys to:@: \
		copy from:ipl20.nas to:@: \
		copy from:make.bat to:@: \
		copy from:hariapp/a.hrb to:@: \
		copy from:hariapp/hello3.hrb to:@: \
		copy from:hariapp/hello4.hrb to:@: \
		copy from:hariapp/hello5.hrb to:@: \
		copy from:hariapp/beepdown.hrb to:@: \
		copy from:hariapp/sosu.hrb to:@: \
		copy from:hariapp/sosu2.hrb to:@: \
		copy from:hariapp/type.hrb to:@: \
		copy from:hariapp/iroha.hrb to:@: \
		copy from:hariapp/chklang.hrb to:@: \
		copy from:euc.txt to:@: \
		copy from:hariapp/calc.hrb to:@: \
		copy from:winhelo/winhelo.hrb to:@: \
		copy from:winhelo2/winhelo2.hrb to:@: \
		copy from:winhelo3/winhelo3.hrb to:@: \
		copy from:star1/star1.hrb to:@: \
		copy from:stars/stars.hrb to:@: \
		copy from:stars2/stars2.hrb to:@: \
		copy from:lines/lines.hrb to:@: \
		copy from:walk/walk.hrb to:@: \
		copy from:noodle/noodle.hrb to:@: \
		copy from:color/color.hrb to:@: \
		copy from:color2/color2.hrb to:@: \
		copy from:sosu3/sosu3.hrb to:@: \
		copy from:notrec/notrec.hrb to:@: \
		copy from:bball/bball.hrb to:@: \
		copy from:invader/invader.hrb to:@: \
		copy from:tview/tview.hrb to:@: \
		copy from:mmlplay/mmlplay.hrb to:@: \
		copy from:mmldata/kirakira.mml to:@: \
		copy from:mmldata/fujisan.mml to:@: \
		copy from:mmldata/daigo.mml to:@: \
		copy from:mmldata/daiku.mml to:@: \
		copy from:gview/gview.hrb to:@: \
		copy from:vclock/vclock.hrb to:@: \
		copy from:cpuid/cpuid.hrb to:@: \
		copy from:puzzle/puzzle.hrb to:@: \
		copy from:pictdata/fujisan.jpg to:@: \
		copy from:pictdata/night.bmp to:@: \
		copy from:pictdata/vclock.bmp to:@: \
		copy from:pictdata/rapuos.bmp to:@: \
		copy from:pictdata/rapuwp.jpg to:@: \
		copy from:nihongo/nihongo.fnt to:@: \
		imgout:rapuos.img

# コマンド

run :
	$(MAKE) rapuos.img
	$(COPY) rapuos.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) rapuos.img
	$(IMGTOL) w a: rapuos.img

full :
	$(MAKE) -C haribote
	$(MAKE) -C apilib
	$(MAKE) -C winhelo
	$(MAKE) -C winhelo2
	$(MAKE) -C winhelo3
	$(MAKE) -C star1
	$(MAKE) -C stars
	$(MAKE) -C stars2
	$(MAKE) -C lines
	$(MAKE) -C walk
	$(MAKE) -C noodle
	$(MAKE) -C color
	$(MAKE) -C color2
	$(MAKE) -C sosu3
	$(MAKE) -C notrec
	$(MAKE) -C bball
	$(MAKE) -C invader
	$(MAKE) -C tview
	$(MAKE) -C mmlplay
	$(MAKE) -C gview
	$(MAKE) -C vclock
	$(MAKE) -C cpuid
	$(MAKE) -C puzzle
	$(MAKE) rapuos.img

run_full :
	$(MAKE) full
	$(COPY) rapuos.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install_full :
	$(MAKE) full
	$(IMGTOL) w a: rapuos.img

run_os :
	$(MAKE) -C haribote
	$(MAKE) run

clean :
# 何もしない

src_only :
	$(MAKE) clean
	-$(DEL) rapuos.img

clean_full :
	$(MAKE) -C haribote		clean
	$(MAKE) -C apilib		clean
	$(MAKE) -C winhelo		clean
	$(MAKE) -C winhelo2		clean
	$(MAKE) -C winhelo3		clean
	$(MAKE) -C star1		clean
	$(MAKE) -C stars		clean
	$(MAKE) -C stars2		clean
	$(MAKE) -C lines		clean
	$(MAKE) -C walk			clean
	$(MAKE) -C noodle		clean
	$(MAKE) -C color		clean
	$(MAKE) -C color2		clean
	$(MAKE) -C sosu3		clean
	$(MAKE) -C notrec		clean
	$(MAKE) -C bball		clean
	$(MAKE) -C invader		clean
	$(MAKE) -C tview		clean
	$(MAKE) -C mmlplay		clean
	$(MAKE) -C gview		clean
	$(MAKE) -C vclock		clean
	$(MAKE) -C cpuid		clean
	$(MAKE) -C puzzle		clean

src_only_full :
	$(MAKE) -C haribote		src_only
	$(MAKE) -C apilib		src_only
	$(MAKE) -C winhelo		src_only
	$(MAKE) -C winhelo2		src_only
	$(MAKE) -C winhelo3		src_only
	$(MAKE) -C star1		src_only
	$(MAKE) -C stars		src_only
	$(MAKE) -C stars2		src_only
	$(MAKE) -C lines		src_only
	$(MAKE) -C walk			src_only
	$(MAKE) -C noodle		src_only
	$(MAKE) -C color		src_only
	$(MAKE) -C color2		src_only
	$(MAKE) -C sosu3		src_only
	$(MAKE) -C notrec		src_only
	$(MAKE) -C bball		src_only
	$(MAKE) -C invader		src_only
	$(MAKE) -C tview		src_only
	$(MAKE) -C mmlplay		src_only
	$(MAKE) -C gview		src_only
	$(MAKE) -C vclock		src_only
	$(MAKE) -C cpuid		src_only
	$(MAKE) -C puzzle		src_only
	-$(DEL) rapuos.img

refresh :
	$(MAKE) full
	$(MAKE) clean_full
	-$(DEL) rapuos.img
