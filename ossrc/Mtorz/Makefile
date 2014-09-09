TOOLPATH = ../z_tools/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# デフォルト動作

default :
	$(MAKE) img


ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

mtorz.com : mtorz.nas Makefile
	$(NASK) mtorz.nas mtorz.com mtorz.lst

mtorz.img : ipl.bin mtorz.com Makefile
	$(COPY) /B ipl.bin+mtorz.com mtorz_.img
	$(IMGTOL) e mtorz_.img mtorz.img 1440


# コマンド
img :
	$(MAKE) mtorz.img

run :
	$(MAKE) mtorz.img
	$(COPY) mtorz.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

clean :
	$(DEL) *.lst
	$(DEL) *.img
