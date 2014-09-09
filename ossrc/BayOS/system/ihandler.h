#if !defined(_BAYOS_UTIL_INTERRUPTHANDLER_H_INCLUDED_)
#define _BAYOS_UTIL_INTERRUPTHANDLER_H_INCLUDED_

#define ADR_BOOTINFO 0x00000ff0
#define ADR_DISKIMG  0x00100000

/* C言語の関数宣言 */
extern "C"
{
    /* ブート時のパラメータ (0x0ff0-0x0fff) */
    typedef struct BOOT_PARAM
    {
        char cyls;     /* ブートセクタはどこまでディスクを読んだのか */
        char leds;     /* ブート時のキーボードのLEDの状態 */
        char bpps;     /* ビデオモード  何ビットカラーか */
        char reserve;  /* 予約 */
        unsigned short screen_x; /* 画面解像度 */
        unsigned short screen_y; /* 画面解像度 */
        unsigned char  *vram;    /* VRAMのアドレス */
    };

    /* I/O系 */
    void io_hlt(void);
    void io_cli(void);
    void io_sti(void);
    void io_stihlt(void);
    int  io_in8(int port);
    int  io_in16(int port);
    int  io_in32(int port);
    void io_out8(int port, int data);
    void io_out16(int port, int data);
    void io_out32(int port, int data);
    int  io_load_eflags(void);
    void io_store_eflags(int eflags);
    void load_gdtr(int limit, int addr);
    void load_idtr(int limit, int addr);
    int  load_cr0(void);
    void store_cr0(int cr0);
    void load_tr(int tr);

    /* ユーティリティ系 */
    unsigned int get_physical_size(unsigned int start, unsigned int end);
    void farjmp(int eip, int cs);
    void farcall(int eip, int cs);

    /* 割り込み系 */
    void asm_inthandler0c(void);
    void asm_inthandler0d(void);
    void asm_inthandler20(void);
    void asm_inthandler21(void);
    void asm_inthandler2c(void);

    /* システムコール系 */
    void asm_hrb_api(void);
    void asm_end_app(void);

    /* 割り込み系 */
    int* inthandler0d(int *esp);
    int* inthandler0c(int *esp);
    void inthandler20(int *esp);
    void inthandler21(int *esp);
    void inthandler2c(int *esp);
    void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);

    /* システムコール系 */
    int* hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
};

#endif
