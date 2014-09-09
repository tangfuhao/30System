OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj window.obj console.obj file.obj tek.obj jpeg.obj alloca.obj

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/
APILIBPATH   = ../apilib/

APP = cpuid

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
CPP      = $(TOOLPATH)c++.exe -I../z_tools/haribote -Os -Wall -nostdlib -fno-builtin -fno-exceptions -fno-rtti -B../z_tools -c
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
GOLIB    = $(TOOLPATH)golib00.exe 
COPY     = copy
DEL      = del
OSACMP   = $(TOOLPATH)bim2bin.exe -osacmp

# デフォルト動作

default :
	$(MAKE) ipl20.bin
	$(MAKE) haribote.sys

# ファイル生成規則

ipl20.bin : ipl20.nas Makefile
	$(NASK) ipl20.nas ipl20.bin ipl20.lst

asmhead.bin : asmhead_64k.nas Makefile
	$(NASK) asmhead_64k.nas asmhead.bin asmhead.lst

#asmhead.bin : asmhead_256.nas Makefile
#	$(NASK) asmhead_256.nas asmhead.bin asmhead.lst

hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

haribote.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb haribote.sys

hiyos.img : ipl20.bin haribote.sys Makefile
	$(EDIMG) imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl20.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		copy from:nihongo.fnt to:@: \
		copy from:cpuid.hrb to:@: \
		copy from:hrbwall.jpg to:@: \
		imgout:hiyos.img

# 一般規則

%.gas : %.c bootpack.h Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

%.obj : %.cpp Makefile
	$(CPP) -o $*.obj $*.cpp


$(APP).bim : $(APP).obj $(APILIBPATH)apilib.lib Makefile
	$(OBJ2BIM) @$(RULEFILE) out:$(APP).bim map:$(APP).map stack:$(STACK) \
		$(APP).obj $(ASMLIB).obj $(APILIBPATH)apilib.lib



# コマンド

clean :
	-$(DEL) asmhead.bin
	-$(DEL) hankaku.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.map
	-$(DEL) *.bim
	-$(DEL) *.hrb
	-$(DEL) *.bin
	-$(DEL) *.sys
	-$(DEL) *.img

img :
	$(MAKE) hiyos.img

run :
	$(MAKE) img
	$(COPY) hiyos.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: hiyos.img
