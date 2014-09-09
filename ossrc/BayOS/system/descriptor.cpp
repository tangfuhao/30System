#include "../bayos.h"

/* �f�X�N���v�^�̏����� */
void Descriptor::init(void)
{
    SEGMENT_DESCRIPTOR* gdt = (SEGMENT_DESCRIPTOR *) ADDRESS_GDT;
    GATE_DESCRIPTOR*    idt = (GATE_DESCRIPTOR    *) ADDRESS_IDT;

    /*
      GDT�̏�����
      0x00000000 - 0xffffffff -- �J�[�l���f�[�^�Z�O�����g (4GB)
      0x00280000 - 0x002fffff -- �J�[�l���R�[�h�Z�O�����g (512KB)
    */
    for (int i = 0; i <= LIMIT_GDT / 8; i++)
    {
        setSegmentDescriptor(gdt + i, 0, 0, 0);
    }
    setSegmentDescriptor(gdt + 1, 0xffffffff,   0x00000000,     AR_DATA32_RW);
    setSegmentDescriptor(gdt + 2, LIMIT_KERNEL, ADDRESS_KERNEL, AR_CODE32_ER);
    load_gdtr(LIMIT_GDT, ADDRESS_GDT);

    /* IDT�̏����� */
    for (int i = 0; i <= LIMIT_IDT / 8; i++)
    {
        setGateDescriptor(idt + i, 0, 0, 0);
    }
    load_idtr(LIMIT_IDT, ADDRESS_IDT);

    /*
      IDT�̐ݒ�
      asm_inthandler0c -- �X�^�b�N��O
      asm_inthandler0d -- ��ʕی��O
      asm_inthandler20 -- �^�C�}�[���荞��
      asm_inthandler21 -- PS/2�L�[�{�[�h���荞��
      asm_inthandler2c -- PS/2�}�E�X���荞��
      asm_hrb_api      -- �V�X�e���R�[��
    */
    setGateDescriptor(idt + 0x0c, (int) asm_inthandler0c, 2 * 8, AR_INTGATE32);
    setGateDescriptor(idt + 0x0d, (int) asm_inthandler0d, 2 * 8, AR_INTGATE32);
    setGateDescriptor(idt + 0x20, (int) asm_inthandler20, 2 * 8, AR_INTGATE32);
    setGateDescriptor(idt + 0x21, (int) asm_inthandler21, 2 * 8, AR_INTGATE32);
    setGateDescriptor(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32);
    setGateDescriptor(idt + 0x40, (int) asm_hrb_api,      2 * 8, AR_INTGATE32 + 0x60);

    return;
}

/* �Z�O�����g�f�X�N���v�^��ݒ肷�� */
void Descriptor::setSegmentDescriptor(SEGMENT_DESCRIPTOR* sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff)
    {
        ar |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }

    sd->limit_low    = limit & 0xffff;
    sd->base_low     = base & 0xffff;
    sd->base_mid     = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high    = (base >> 24) & 0xff;

    return;
}

/* ���荞�݃f�X�N���v�^��ݒ肷�� */
void Descriptor::setGateDescriptor(GATE_DESCRIPTOR* gd, int offset, int selector, int ar)
{
    gd->offset_low   = offset & 0xffff;
    gd->selector     = selector;
    gd->dw_count     = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high  = (offset >> 16) & 0xffff;

    return;
}
