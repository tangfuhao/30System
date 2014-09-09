#if !defined(_BAYOS_UTIL_MEMORY_H_INCLUDED_)
#define _BAYOS_UTIL_MEMORY_H_INCLUDED_

namespace system
{
    /* �������}�l�[�W���[ */
    class Memory
    {
    private:
        dword start, end, used, firstFree;

    public:
        void  init(dword start, dword end);
        void* allocate(dword size);
        void  free(void* address);
        dword getTotalSize();
        dword getFreeSize();

    private:
        dword searchFit(dword size);
        bool  checkAddress(dword ptr, dword* prev);
        void  setNext(dword ptr, dword next);
        dword getNext(dword ptr);
    };
}

using namespace system;

//
// �������牺�̓������[����p�̊֐�
//

/* C����̊֐��錾 */
extern "C"
{
    void* memcpy(void* s1, const void* s2, size_t size);
};

//
// �������牺�� C++����� new / delete �T�|�[�g
// ����� C����� malloc / free �T�|�[�g
//

extern Memory g_km;

void* operator new(size_t size);
void* operator new[](size_t size);
void  operator delete(void* address);
void  operator delete[](void* address);

/* C����̊֐��錾 */
extern "C"
{
    void* malloc(size_t size);
    void  free(void* address);
};

#endif
