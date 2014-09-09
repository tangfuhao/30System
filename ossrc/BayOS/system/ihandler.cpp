#include "../bayos.h"
#include "../apilib/syscall.h"

/* タイマーハンドラ */
void inthandler20(int *esp)
{
    io_out8(PIC0_OCW2, 0x60);   /* IRQ-00受付完了をPICに通知 */
    PIT::count++;               /* カウントアップ */
    if (Scheduler::ready == true)
    {
        Scheduler::switchTask(); /* 10msごとにタスクスイッチを行う */
    }
    return;
}

/* キーボードハンドラ */
void inthandler21(int *esp)
{
    io_out8(PIC0_OCW2, 0x61);   /* IRQ-01受付完了をPICに通知 */
    int data = io_in8(PORT_KEYDAT);
    Scheduler::task_list[TASK_KEYBOARD]->fifo->add(data);
    return;
}

/* マウスハンドラ */
void inthandler2c(int *esp)
{
    io_out8(PIC1_OCW2, 0x64);   /* IRQ-12受付完了をPIC1に通知 */
    io_out8(PIC0_OCW2, 0x62);   /* IRQ-02受付完了をPIC0に通知 */
    int data = io_in8(PORT_KEYDAT);
    Scheduler::task_list[TASK_MOUSE]->fifo->add(data);
    return;
}

/* スタック例外 */
int *inthandler0d(int *esp)
{
    Console::printf("stack exception esp=0x%08x thread=%s\n",   esp, Scheduler::current->name);
    Console::printf("EDI=%08x\tESI=%08x\tEBP=%08x\tEBX=%08x\n", esp[0x0], esp[0x1], esp[0x2], esp[0x4]);
    Console::printf("EDX=%08x\tECX=%08x\tEAX=%08x\tDS =%08x\n", esp[0x5], esp[0x6], esp[0x7], esp[0x8]);
    Console::printf("ES =%08x\tEIP=%08x\tCS =%08x\tFLG=%08x\n", esp[0x9], esp[0xb], esp[0xc], esp[0xd]);
    Console::printf("ESP=%08x\tSS =%08x\n",                     esp[0xe], esp[0xf]);
    return &(Scheduler::current->tss.esp0); /* 異常終了させる */
}

/* 一般保護例外 */
int *inthandler0c(int *esp)
{
    Console::printf("general protection esp=0x%08x thread=%s\n", esp, Scheduler::current->name);
    Console::printf("EDI=%08x\tESI=%08x\tEBP=%08x\tEBX=%08x\n",  esp[0x0], esp[0x1], esp[0x2], esp[0x4]);
    Console::printf("EDX=%08x\tECX=%08x\tEAX=%08x\tDS =%08x\n",  esp[0x5], esp[0x6], esp[0x7], esp[0x8]);
    Console::printf("ES =%08x\tEIP=%08x\tCS =%08x\tFLG=%08x\n",  esp[0x9], esp[0xb], esp[0xc], esp[0xd]);
    Console::printf("ESP=%08x\tSS =%08x\n",                      esp[0xe], esp[0xf]);
    return &(Scheduler::current->tss.esp0); /* 異常終了させる */
}

/* システムコール */
int *hrb_api(int arg3, int arg1, int arg5, int arg6, int func_num, int arg4, int arg2, int result)
{
    Task* current = Scheduler::current;
    int ds_base   = current->ds_base;

    /*
      保存のためのPUSHADを強引に書き換える
      システムコールの戻り値は reg[7] に設定すること
      reg[0] : EDI, reg[1] : ESI, reg[2] : EBP, reg[3] : ESP
      reg[4] : EBX, reg[5] : EDX, reg[6] : ECX, reg[7] : EAX
    */
    int* reg = &result + 1;

    if (func_num == API_PUTCHAR)
    {
        Console::putChar(arg1 & 0xff);
    }

    else if (func_num == API_PUTSTR0)
    {
        char* s = (char *) arg1 + ds_base;
        Console::putString(s);
    }

    else if (func_num == API_PUTSTR1)
    {
        char* s = (char *) arg1 + ds_base;
        for (int i = 0; i <= arg2; i++)
        {
            Console::putChar(s[i] & 0xff);
        }
    }

    else if (func_num == API_END)
    {
        return &(current->tss.esp0);
    }

    else if (func_num == API_OPENWIN)
    {
        byte* buffer = (byte *) arg1 + ds_base;
        int w = (arg2 >> 16) & 0xffff;
        int h = arg2 & 0xffff;
        int transparent = arg3;
        char* title = (char *) arg4 + ds_base;
        Sheet* sheet = SheetCtl::allocate();
        SheetCtl::setbuf(sheet, (word *) buffer, w, h, transparent);
        sheet->vx0 = (SheetCtl::ctl->xsize - w) / 2;
        sheet->vy0 = (SheetCtl::ctl->ysize - h) / 2;
        SheetCtl::drawFrame(sheet, title);
        SheetCtl::updown(sheet, 2);
        reg[7] = (int) sheet;
    }

    else if (func_num == API_PUTSTRWIN)
    {
#if 0
        Sheet* sheet = (Sheet *) arg1;
        int x   = (arg2 >> 16) & 0xffff;
        int y   = arg2 & 0xffff;
        int col = arg3;
        char* s = (char *) arg4 + ds_base;
        // TODO
#endif
    }

    else if (func_num == API_BOXFILWIN)
    {
        Sheet* sheet = (Sheet *) arg1;
        int x0  = (arg2 >> 16) & 0xffff;
        int y0  = arg2 & 0xffff;
        int x1  = (arg3 >> 16) & 0xffff;
        int y1  = arg3 & 0xffff;
        int col = arg4;
        SheetCtl::fillRect(sheet, x0, y0, x1, y1, col);
    }

    else if (func_num == API_MALLOC)
    {
        void* addr = current->memory->allocate(arg1);
        reg[7] = ((int) addr) - ds_base;
    }

    else if (func_num == API_FREE)
    {
        dword* addr = (dword *) arg1 + ds_base;
        current->memory->free(addr);
    }

    else if (func_num == API_POINT)
    {
        Sheet* sheet = (Sheet *) arg1;
        int x0  = arg2;
        int y0  = arg3;
        int col = arg4;
        SheetCtl::drawPixel(sheet, x0, y0, col);
    }

    else if (func_num == API_REFRESHWIN)
    {
        Sheet* sheet = (Sheet *) arg1;
        int x0 = (arg2 >> 16) & 0xffff;
        int y0 = arg2 & 0xffff;
        int x1 = (arg3 >> 16) & 0xffff;
        int y1 = arg3 & 0xffff;
        SheetCtl::refresh(sheet, x0, y0, x1, y1);
    }

    else if (func_num == API_LINEWIN)
    {
        Sheet* sheet = (Sheet *) arg1;
        int x0  = (arg2 >> 16) & 0xffff;
        int y0  = arg2 & 0xffff;
        int x1  = (arg3 >> 16) & 0xffff;
        int y1  = arg3 & 0xffff;
        int col = arg4;
        SheetCtl::drawLine(sheet, x0, y0, x1, y1, col);
    }

    else if (func_num == API_CLOSEWIN)
    {
        Sheet* sheet = (Sheet *) arg1;
        SheetCtl::dispose(sheet);
        delete(sheet->buffer);
        delete(sheet);
    }

    else if (func_num == API_GETKEY)
    {
        List<int>* fifo = Scheduler::task_list[TASK_KEYBOARD]->fifo;
        if (fifo->isEmpty() == false)
        {
            int i = fifo->removeAt(0);
            char c = 0;
            if ((c = Keyboard::decode(i)) != 0)
            {
                reg[7] = c;
            }
            else
            {
                reg[7] = 0;
            }
        }
        else
        {
            reg[7] = 0;
        }
    }

    else if (func_num == API_BEEP)
    {
        Beep::tone(arg1);
    }

    else if (func_num == API_FOPEN)
    {
        char* s = (char *) arg1 + ds_base;
        FileEntry* entry = File::load(s);
        reg[7] = (int) entry;
    }

    else if (func_num == API_FCLOSE)
    {
        //FileEntry* entry = (FileEntry *) arg1;
        //delete(entry);
    }

    else if (func_num == API_FSIZE)
    {
        FileEntry* entry = (FileEntry *) arg1;
        if (entry != NULL)
        {
            reg[7] = entry->size;
        }
        else
        {
            reg[7] = 0;
        }
    }

    else if (func_num == API_FREAD)
    {
        char* s = (char *) arg1 + ds_base;
        int maxsize = arg2;
        FileEntry* entry = (FileEntry *) arg3;
        if (entry != NULL)
        {
            memcpy(s, entry->data, maxsize);
            reg[7] = entry->size;
        }
        else
        {
            reg[7] = 0;
        }
    }

    return 0; /* 0以外が返るとアプリが強制終了される */
}
