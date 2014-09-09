#include "../bayos.h"

/* �A�C�h���^�X�N */
void task_idle(void)
{
    for (;;)
    {
        io_hlt();
    }
}

/* �L�[�{�[�h�^�X�N */
void task_keyboard(void)
{
    for (;;)
    {
        io_cli(); /* ���荞�݂��֎~���� */
        List<int>* fifo = Scheduler::current->fifo;
        if (fifo->isEmpty() == false)
        {
            int  i = fifo->removeAt(0);
            char c = 0;
            if ((c = Keyboard::decode(i)) != 0)
            {
                Console::putKey(c);
            }
        }
        else
        {
            /* FIFO����ɂȂ�����ʂ̃^�X�N�ɐ؂�ւ� */
            Scheduler::switchTask();
        }
        io_cli(); /* ���荞�݂��֎~���� */
    }
}

/* �}�E�X�^�X�N */
void task_mouse(void)
{
    bool drag_flag = false;
    int  mx = 0, my = 0;

    for (;;)
    {
        io_cli(); /* ���荞�݂��֎~���� */
        List<int>* fifo = Scheduler::current->fifo;
        if (fifo->isEmpty() == false)
        {
            int i = fifo->removeAt(0);
            if (Mouse::decode(i) != 0)
            {
                /* �w�肵�����~������ԏォ��Q�Ԗڂɂ����� */
                if (Mouse::state == MOUSE_PRESSED)
                {
                    for (int i = SheetCtl::ctl->top - 1; i > 0; i--)
                    {
                        Sheet* sheet = SheetCtl::ctl->sheets[i];
                        if (sheet->vx0 < Mouse::x && Mouse::x < sheet->vx0 + sheet->width &&
                                sheet->vy0 < Mouse::y && Mouse::y < sheet->vy0 + sheet->height)
                        {
                            /* ��ԏ�Ȃ�h���b�O�Ώۂɂ��� */
                            if (i == SheetCtl::ctl->top - 1)
                            {
                                drag_flag = true;
                                mx = Mouse::x - sheet->vx0;
                                my = Mouse::y - sheet->vy0;
                                break;
                            }
                            if (i < SheetCtl::ctl->top - 1)
                            {
                                SheetCtl::updown(sheet, SheetCtl::ctl->top - 1);
                                break;
                            }
                        }
                    }
                }
                /* ���~���𓮂��� */
                else if (Mouse::state == MOUSE_DRAGGED)
                {
                    if (drag_flag == true)
                    {
                        Sheet* sheet = SheetCtl::ctl->sheets[SheetCtl::ctl->top - 1];
                        SheetCtl::slide(sheet, Mouse::x - mx, Mouse::y - my);
                    }
                }
                /* ���~���̈ړ��I�� */
                else if (Mouse::state == MOUSE_RELEASED)
                {
                    if (drag_flag == true)
                    {
                        drag_flag = false;
                    }
                }
                Mouse::draw();
            }
        }
        else
        {
            /* FIFO����ɂȂ�����ʂ̃^�X�N�ɐ؂�ւ� */
            Scheduler::switchTask();
        }
        io_cli(); /* ���荞�݂��֎~���� */
    }
}

/* �A�v���P�[�V�������s�^�X�N */
#if 0
void task_exec(void)
{
    FileEntry* entry = File::load("noiz2bg.hrb");
    if (entry != NULL)
    {
        dword app_size = entry->size;
        byte* code     = entry->data;

        /* HRB�o�C�i����� */
        if (app_size >= 36 &&
                strncmp((char *) (code + 4), "Hari", 4) == 0 &&
                *code == 0x00)
        {
            int segment_size = *((int *) (code + 0x0000));
            int esp          = *((int *) (code + 0x000c));
            int data_size    = *((int *) (code + 0x0010));
            int data_hrb     = *((int *) (code + 0x0014));
            int malloc_start = *((int *) (code + 0x0020));

            /* �f�o�b�O�p */
            //Console::printf("%d,%x,%d,%x,%x\n", segment_size, esp, data_size, data_hrb, malloc_start);

            /* �f�[�^�Z�O�����g�̊m�ۂƃA�v���p�������}�l�[�W���̏����� */
            byte* data = (byte *)malloc(segment_size);
            Task* current = Scheduler::current;
            current->ds_base = (int) data;
            current->memory  = new Memory();
            current->memory->init((dword) &data[malloc_start], (dword) &data[segment_size - 1]);

            /* DS��CS�̐ݒ� */
            //SEGMENT_DESCRIPTOR* gdt = (SEGMENT_DESCRIPTOR *) ADDRESS_GDT;
            Descriptor::setSegmentDescriptor
            (&current->ldt[0], app_size - 1, (int) code, AR_CODE32_ER + 0x60);
            //(gdt + 1003, app_size - 1, (int) code, AR_CODE32_ER + 0x60);
            Descriptor::setSegmentDescriptor
            (&current->ldt[1], segment_size - 1, (int) data, AR_DATA32_RW + 0x60);
            //(gdt + 1004, segment_size - 1, (int) data, AR_DATA32_RW + 0x60);
            memcpy(&data[esp], &code[data_hrb], data_size);

            /* ���s�J�n */
            start_app(0x1b, 0 * 8 + 4, esp, 1 * 8 + 4, &(current->tss.esp0));
            //start_app(0x1b, 1003 * 8, esp, 1004 * 8, &(current->tss.esp0));

            /* ���s�I�� */
            //delete(entry1);
            //delete(entry2);
            delete(current->memory);
            free(data);
            //free(code);
        }
    }
    Scheduler::runq->remove(Scheduler::current);
    Scheduler::switchTask();
}
#endif

Task** Scheduler::task_list  = NULL;
List<Task*>* Scheduler::runq = NULL;
Task* Scheduler::current     = NULL;
bool Scheduler::ready        = false;

/* �X�P�W���[���̏����� */
void Scheduler::init()
{
    /* �ϐ��̏����� */
    task_list = new Task*[MAX_TASKS];
    runq      = new List<Task*>();

    /* �^�X�N�ꗗ�̏����� */
    SEGMENT_DESCRIPTOR *gdt = (SEGMENT_DESCRIPTOR *) ADDRESS_GDT;
    for (int i = 0; i < MAX_TASKS; i++)
    {
        task_list[i] = new Task();
        task_list[i]->selector = (TASK_GDT_0 + i) * 8;
        task_list[i]->enable   = false;
        task_list[i]->fifo     = new List<int>();
        task_list[i]->tss.ldtr = (TASK_GDT_0 + MAX_TASKS + i) * 8;
        /* �J�[�l���^�X�N�p */
        Descriptor::setSegmentDescriptor
        (gdt + TASK_GDT_0 + i, 103, (int) &task_list[i]->tss, AR_TSS32);
        /* �A�v���P�[�V�����p */
        Descriptor::setSegmentDescriptor
        (gdt + TASK_GDT_0 + MAX_TASKS + i, 15, (int) task_list[i]->ldt, AR_LDT);
    }

    /* ���ݎ��s���̃^�X�N�̐ݒ� */
    current = allocateTask(0, "KERNEL");
    load_tr(current->selector);

    /* �A�C�h���^�X�N�̐ݒ� */
    Task* idle = allocateTask((int) &task_idle, "IDLE");

    /* �L�[�{�[�h�^�X�N�̐ݒ� */
    Task* keyboard = allocateTask((int) &task_keyboard, "KEYBOARD");

    /* �}�E�X�^�X�N�̐ݒ� */
    Task* mouse = allocateTask((int) &task_mouse, "MOUSE");

    /* �A�v���P�[�V�������s�^�X�N�̐ݒ� */
    //Task* appli_1 = allocateTask((int) &task_exec, "WINDOW1");
    //Task* appli_2 = allocateTask((int) &task_exec, "WINDOW2");
    //Task* appli_3 = allocateTask((int) &task_exec, "WINDOW3");

    /* ���s�L���[�Ƀ^�X�N���l�ߍ��� */
    runq->add(idle);
    runq->add(keyboard);
    runq->add(mouse);
    //runq->add(appli_1);
    //runq->add(appli_2);
    //runq->add(appli_3);
    runq->add(current);

    /* �X�P�W���[�����O�\ */
    Scheduler::ready = true;
}

/* �󂢂Ă���^�X�N�̎擾 */
Task* Scheduler::allocateTask(int eip, const char* name)
{
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (task_list[i]->enable == false)
        {
            task_list[i]->enable = true;
            task_list[i]->name   = (char *) malloc(strlen(name) + 1);
            memcpy(task_list[i]->name, name, strlen(name) + 1);
            if (i != 0)
            {
                task_list[i]->tss.esp     = (int) ((byte *)malloc(64 * 1024) + 64 * 1024);
                task_list[i]->tss.eip     = eip;
                task_list[i]->tss.eflags  = 0x00000202; /* IF = 1; */
                task_list[i]->tss.eax     = 0; /* �Ƃ肠����0�ɂ��Ă������Ƃɂ��� */
                task_list[i]->tss.ecx     = 0;
                task_list[i]->tss.edx     = 0;
                task_list[i]->tss.ebx     = 0;
                task_list[i]->tss.ebp     = 0;
                task_list[i]->tss.esi     = 0;
                task_list[i]->tss.edi     = 0;
                task_list[i]->tss.es      = 1 * 8;
                task_list[i]->tss.cs      = 2 * 8;
                task_list[i]->tss.ss      = 1 * 8;
                task_list[i]->tss.ds      = 1 * 8;
                task_list[i]->tss.fs      = 1 * 8;
                task_list[i]->tss.gs      = 1 * 8;
                //task_list[i]->tss.ldtr    = 0;
                task_list[i]->tss.iomap   = 0x40000000;
                task_list[i]->tss.ss0     = 0;
            }
            return task_list[i];
        }
    }
    return NULL;
}

/* ����I�ȃ^�X�N�X�C�b�` */
void Scheduler::switchTask()
{
    current = runq->removeAt(0);
    runq->add(current);
    farjmp(0, current->selector);
}
