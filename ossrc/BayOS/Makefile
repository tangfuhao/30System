OBJS_BOOTPACK = \
	bayos.obj \
	system/descriptor.obj \
	system/console.obj \
	system/ihandler.obj \
	system/memory.obj \
	system/scheduler.obj \
	system/sheet.obj \
	driver/beep.obj \
	driver/clock.obj \
	driver/dtk5s.obj \
	driver/file.obj \
	driver/keyboard.obj \
	driver/mouse.obj \
	driver/pic.obj \
	driver/timer.obj \
	naskfunc.obj

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
CPP      = $(TOOLPATH)c++.exe -I../z_tools/haribote -Os -Wall -nostdlib -fno-builtin -fno-exceptions -fno-rtti -B../z_tools -c
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
BIM2BIN  = $(TOOLPATH)bim2bin.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
MKIMG    = mkimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# デフォルト動作

default :
	$(MAKE) img

# ファイル生成規則

ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bayos.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bayos.bim stack:3136k map:bayos.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bayos.hrb : bayos.bim Makefile
	$(BIM2HRB) bayos.bim bayos.hrb 0

haribote.sys : asmhead.bin bayos.hrb hello.hrb cat.hrb bball.hrb noiz2bg.hrb Makefile
	copy /B asmhead.bin+bayos.hrb haribote.sys

haribote.img : ipl10.bin haribote.sys Makefile
	$(MKIMG) haribote.img \
		ipl10.bin \
		haribote.sys \
		BAYOS.BM5 \
		MONA-12.MN5 \
		license.txt \
		hello.hrb \
		cat.hrb \
		bball.hrb \
		noiz2bg.hrb

# 各種アプリケーション

API_OBJS = \
	apilib/api_putchar.obj \
	apilib/api_putstr0.obj \
	apilib/api_putstr1.obj \
	apilib/api_end.obj \
	apilib/api_openwin.obj \
	apilib/api_putstrwin.obj \
	apilib/api_boxfilwin.obj \
	apilib/api_malloc.obj \
	apilib/api_free.obj \
	apilib/api_point.obj \
	apilib/api_refreshwin.obj \
	apilib/api_linewin.obj \
	apilib/api_closewin.obj \
	apilib/api_getkey.obj \
	apilib/api_beep.obj \
	apilib/api_fopen.obj \
	apilib/api_fclose.obj \
	apilib/api_fsize.obj \
	apilib/api_fread.obj \
	apilib/memcpy.obj \
	apilib/printf.obj \
	apilib/alloca.obj

# hello

apps/hello.bim : apps/hello.obj $(API_OBJS) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:apps/hello.bim map:apps/hello.map stack:1k apps/hello.obj $(API_OBJS)

hello.hrb : apps/hello.bim Makefile
	$(BIM2HRB) apps/hello.bim apps/hello.bin 0
	$(BIM2BIN) in:apps/hello.bin out:hello.hrb -osacmp -tek5 BS:0 eprm:z0

# cat

apps/cat.bim : apps/cat.obj $(API_OBJS) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:apps/cat.bim map:apps/cat.map stack:1k apps/cat.obj $(API_OBJS)

cat.hrb : apps/cat.bim Makefile
	$(BIM2HRB) apps/cat.bim apps/cat.bin 0
	$(BIM2BIN) in:apps/cat.bin out:cat.hrb -osacmp -tek5 BS:0 eprm:z0

# bball

apps/bball.bim : apps/bball.obj $(API_OBJS) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:apps/bball.bim map:apps/bball.map stack:1k apps/bball.obj $(API_OBJS)

bball.hrb : apps/bball.bim Makefile
	$(BIM2HRB) apps/bball.bim apps/bball.bin 128k
	$(BIM2BIN) in:apps/bball.bin out:bball.hrb -osacmp -tek5 BS:0 eprm:z0

# noiz2bg

apps/noiz2bg.bim : apps/noiz2bg.obj $(API_OBJS) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:apps/noiz2bg.bim map:apps/noiz2bg.map stack:1k apps/noiz2bg.obj $(API_OBJS)

noiz2bg.hrb : apps/noiz2bg.bim Makefile
	$(BIM2HRB) apps/noiz2bg.bim apps/noiz2bg.bin 512k
	$(BIM2BIN) in:apps/noiz2bg.bin out:noiz2bg.hrb -osacmp -tek5 BS:0 eprm:z0

# infones

INFONES_OBJS = \
	infones/K6502.obj \
	infones/InfoNES.obj \
	infones/InfoNES_Mapper.obj \
	infones/InfoNES_pAPU.obj \
	infones/InfoNES_System_HariboteOS.obj

infones/infones.bim : $(INFONES_OBJS) $(API_OBJS) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:infones/infones.bim map:infones/infones.map stack:1k $(INFONES_OBJS) $(API_OBJS)

infones.hrb : infones/infones.bim Makefile
	$(BIM2HRB) infones/infones.bim infones/infones.bin 1024k
	$(BIM2BIN) in:infones/infones.bin out:infones.hrb -osacmp -tek5 BS:0 eprm:z0

# 一般規則

%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

%.obj : %.cpp Makefile
	$(CPP) -o $*.obj $*.cpp

# コマンド

img :
	$(MAKE) haribote.img

iso :
	mkisofs -v -iso-level 1 -b haribote.img -o haribote.iso .

run :
	$(MAKE) img
	$(COPY) haribote.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: haribote.img

clean :
	-$(DEL) *.bim
	-$(DEL) *.bin
	-$(DEL) *.hrb
	-$(DEL) *.lst
	-$(DEL) *.map
	-$(DEL) *.obj
	-$(DEL) *.log
	-$(DEL) *.nvram
	-$(DEL) *.vmsd
	-$(DEL) apps\*.bim
	-$(DEL) apps\*.bin
	-$(DEL) apps\*.map
	-$(DEL) apps\*.obj
	-$(DEL) apilib\*.lst
	-$(DEL) apilib\*.obj
	-$(DEL) driver\*.obj
	-$(DEL) system\*.obj
	-$(DEL) haribote.img
	-$(DEL) haribote.sys

depend : 
	gcc -MM baysos.cpp system/*.cpp > dependencies

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img
