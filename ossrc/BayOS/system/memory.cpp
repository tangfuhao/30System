#include "../bayos.h"

/* �������G���g���[ */
class MemoryEntry
{
public:
    int Size;
    dword Prev, Next;

    inline static void write_used(dword ptr, dword size)
    {
        *(dword*)ptr = *(dword*)(ptr + size - sizeof(dword)) = size;
    }

    inline static void write_free(dword ptr, dword size, dword prev, dword next)
    {
        dword* p = (dword*)ptr;
        *(p++) = *(dword*)(ptr + size - sizeof(dword)) = size | 0x80000000;
        *(p++) = prev;
        *p = next;
    }

    inline static void write_last(dword ptr, dword prev)
    {
        dword* p = (dword*)ptr;
        *(p++) = 0;
        *p = prev;
    }
};

/* �������}�l�[�W���[�̏����� */
void Memory::init(dword start, dword end)
{
    this->start = this->firstFree = start;
    this->end = end;
    this->used = 0;
    MemoryEntry::write_last(this->start, 0);
}

/* �������̊��蓖�� */
void* Memory::allocate(dword size)
{
    dword sz = (sizeof(dword) + size + sizeof(dword) + 15) & 0xfffffff0; // align16
    dword ptr = this->searchFit(sz);
    if (ptr == 0 || ptr + sz > this->end) return NULL;
    this->used += sz;

    MemoryEntry* h = (MemoryEntry*)ptr;
    dword sz_old = h->Size & 0x7fffffff;
    dword nptr = ptr + sz, prev = h->Prev, next = nptr;
    if (sz_old == 0)
    {
        MemoryEntry::write_last(nptr, prev);
    }
    else
    {
        if (sz < sz_old)
        {
            MemoryEntry::write_free(nptr, sz_old - sz, prev, h->Next);
            ((MemoryEntry*)h->Next)->Prev = nptr;
        }
        else
        {
            next = h->Next;
            ((MemoryEntry*)next)->Prev = prev;
        }
    }
    MemoryEntry::write_used(ptr, sz);
    this->setNext(prev, next);
    return (void*)(ptr + sizeof(dword));
}

/* �󂢂Ă��郁������T�� */
dword Memory::searchFit(dword size)
{
    for (dword p = this->firstFree; p <= this->end; p = ((MemoryEntry*)p)->Next)
    {
        int sz = ((MemoryEntry*)p)->Size;
        if (sz == 0)
        {
            return p;
        }
        else if (sz > 0)
        {
            break;
        }
        sz &= 0x7fffffff;
        if (size <= (dword)sz) return p;
    }
    return 0;
}

/* ��������������� */
void Memory::free(void* address)
{
    dword ptr = ((dword)address) - sizeof(dword), prev = 0;
    if (!this->checkAddress(ptr, &prev)) return;
    this->used -= ((MemoryEntry*)ptr)->Size;

    dword sz = *(int*)ptr, next = this->getNext(prev);
    if (ptr > this->start)
    {
        int psz = *(int*)(ptr - sizeof(int));
        if (psz < 0)
        {
            psz &= 0x7fffffff;
            ptr -= psz;
            sz  += psz;
            prev = ((MemoryEntry*)ptr)->Prev;
        }
    }
    dword nptr = ptr + sz, nsz = *(dword*)nptr;
    if (nsz < 0)
    {
        sz += nsz & 0x7fffffff;
        next = ((MemoryEntry*)nptr)->Next;
    }

    if (nsz == 0)
    {
        MemoryEntry::write_last(ptr, prev);
    }
    else
    {
        MemoryEntry::write_free(ptr, sz, prev, next);
        ((MemoryEntry*)next)->Prev = ptr;
    }
    this->setNext(prev, ptr);
}

/* �A�h���X���`�F�b�N���� */
bool Memory::checkAddress(dword ptr, dword* prev)
{
    if (ptr < this->start || ptr > this->end) return false;

    int psz1 = *(int*)ptr;
    if (psz1 < 16 || (psz1 & 15) != 0 || ptr + psz1 > this->end) return false;

    int psz2 = *(int*)(ptr + psz1 - sizeof(int));
    if (psz1 != psz2) return false;

    for (dword p = this->firstFree; p <= ptr; p = ((MemoryEntry*)p)->Next)
    {
        if (((MemoryEntry*)p)->Size >= 0) return false;
        *prev = p;
    }
    return true;
}

/* ���̃������G���g����ݒ肷�� */
void Memory::setNext(dword ptr, dword next)
{
    if (ptr == 0)
    {
        this->firstFree = next;
    }
    else
    {
        ((MemoryEntry*)ptr)->Next = next;
    }
}

/* ���̃������G���g���𓾂� */
dword Memory::getNext(dword ptr)
{
    return ptr == 0 ? this->firstFree : ((MemoryEntry*)ptr)->Next;
}

/* ���T�C�Y�𓾂� */
dword Memory::getTotalSize()
{
    return (this->end - this->start);
}

/* �󂢂Ă���T�C�Y�𓾂� */
dword Memory::getFreeSize()
{
    return (this->end - this->start - this->used);
}

//
// �������牺�̓������[����p�̊֐�
//

void* memcpy(void* s1, const void* s2, size_t size)
{
    asm volatile("movl %0, %%edi \n"
                 "movl %1, %%esi \n"
                 "movl %2, %%ecx \n"
                 "cld            \n"
                 "rep            \n"
                 "movsb          \n"
                 : /* no output */
                 : "m"(s1), "m"(s2), "m"(size)
                 : "edi", "esi", "ecx");

    return s1;
}

//
// �������牺�� C++����� new / delete �T�|�[�g
// ����� C����� malloc / free �T�|�[�g
//

void* operator new(size_t size)
{
    void* p = g_km.allocate(size);
    if (p)
    {
        memset(p, 0, size);
    }
    return p;
}

void operator delete(void* address)
{
    if (address)
    {
        g_km.free(address);
    }
    return;
}

void* operator new[](size_t size)
{
    void* p = g_km.allocate(size);
    if (p)
    {
        memset(p, 0, size);
    }
    return p;
}

void operator delete[](void* address)
{
    if (address)
    {
        g_km.free(address);
    }
    return;
}

void* malloc(size_t size)
{
    void* p = g_km.allocate(size);
    if (p)
    {
        memset(p, 0, size);
    }
    return p;
}

void free(void* address)
{
    if (address)
    {
        g_km.free(address);
    }
    return;
}
