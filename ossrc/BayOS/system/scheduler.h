#if !defined(_BAYOS_UTIL_SCHEDULER_H_INCLUDED_)
#define _BAYOS_UTIL_SCHEDULER_H_INCLUDED_

#define MAX_TASKS       64    /* 最大タスク数 */
#define TASK_GDT_0      3     /* TSSをGDTの何番から割り当てるのか */
#define TASK_KEYBOARD   2     /* キーボードタスクの番号 */
#define TASK_MOUSE      3     /* マウスタスクの番号 */

/* C言語の関数宣言 */
extern "C"
{
    /* アイドルタスク */
    void task_idle(void);

    /* キーボードタスク */
    void task_keyboard(void);

    /* マウスタスク */
    void task_mouse(void);

    /* 32ビットTSS */
    typedef struct TSS32
    {
        int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
        int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
        int es, cs, ss, ds, fs, gs;
        int ldtr, iomap;
    };

    /* タスク構造体 */
    typedef struct Task
    {
        int   selector;  /* GDTの番号 */
        bool  enable;    /* 動作フラグ */
        char* name;      /* タスク名 */
        TSS32 tss;       /* 32ビットTSS */
        int   ds_base;   /* データセグメント開始アドレス */
        List<int>* fifo; /* FIFOバッファ */
        SEGMENT_DESCRIPTOR ldt[2]; /* ローカルデスクリプタ */
        Memory* memory;  /* アプリ用メモリマネージャ */
    };
}

namespace system
{
    /* タスクスケジューラー */
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
