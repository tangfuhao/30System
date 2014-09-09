#if !defined(_BAYOS_UTIL_DESCRIPTOR_H_INCLUDED_)
#define _BAYOS_UTIL_DESCRIPTOR_H_INCLUDED_

#define ADDRESS_IDT     0x0026f800
#define LIMIT_IDT       0x000007ff
#define ADDRESS_GDT     0x00270000
#define LIMIT_GDT       0x0000ffff
#define ADDRESS_KERNEL  0x00280000
#define LIMIT_KERNEL    0x0007ffff
#define AR_DATA32_RW    0x4092
#define AR_CODE32_ER    0x409a
#define AR_LDT          0x0082
#define AR_TSS32        0x0089
#define AR_INTGATE32    0x008e

/* C言語の関数宣言 */
extern "C"
{
    /* セグメントデスクリプタ */
    typedef struct SEGMENT_DESCRIPTOR
    {
        short limit_low, base_low;
        char  base_mid, access_right;
        char  limit_high, base_high;
    };

    /* 割り込みデスクリプタ */
    typedef struct GATE_DESCRIPTOR
    {
        short offset_low, selector;
        char  dw_count, access_right;
        short offset_high;
    };
}

namespace system
{
    /* デスクリプタ */
    class Descriptor
    {
    public:
        static void init(void);
        static void setSegmentDescriptor(SEGMENT_DESCRIPTOR* sd, unsigned int limit, int base, int ar);
        static void setGateDescriptor(GATE_DESCRIPTOR* gd, int offset, int selector, int ar);
    };
}

#endif
