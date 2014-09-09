#if !defined(_BAYOS_UTIL_SCHEDULER_H_INCLUDED_)
#define _BAYOS_UTIL_SCHEDULER_H_INCLUDED_

#define MAX_TASKS       64    /* �ő�^�X�N�� */
#define TASK_GDT_0      3     /* TSS��GDT�̉��Ԃ��犄�蓖�Ă�̂� */
#define TASK_KEYBOARD   2     /* �L�[�{�[�h�^�X�N�̔ԍ� */
#define TASK_MOUSE      3     /* �}�E�X�^�X�N�̔ԍ� */

/* C����̊֐��錾 */
extern "C"
{
    /* �A�C�h���^�X�N */
    void task_idle(void);

    /* �L�[�{�[�h�^�X�N */
    void task_keyboard(void);

    /* �}�E�X�^�X�N */
    void task_mouse(void);

    /* 32�r�b�gTSS */
    typedef struct TSS32
    {
        int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
        int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
        int es, cs, ss, ds, fs, gs;
        int ldtr, iomap;
    };

    /* �^�X�N�\���� */
    typedef struct Task
    {
        int   selector;  /* GDT�̔ԍ� */
        bool  enable;    /* ����t���O */
        char* name;      /* �^�X�N�� */
        TSS32 tss;       /* 32�r�b�gTSS */
        int   ds_base;   /* �f�[�^�Z�O�����g�J�n�A�h���X */
        List<int>* fifo; /* FIFO�o�b�t�@ */
        SEGMENT_DESCRIPTOR ldt[2]; /* ���[�J���f�X�N���v�^ */
        Memory* memory;  /* �A�v���p�������}�l�[�W�� */
    };
}

namespace system
{
    /* �^�X�N�X�P�W���[���[ */
    class Scheduler
    {
    public:
        static Task** task_list;
        static List<Task*>* runq;
        static Task* current;
        static bool ready;

    public:
        static void  init(void);
        static Task* allocateTask(int eip, const char* name);
        static void  switchTask(void);
    };
}

#endif
