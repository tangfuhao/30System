#if !defined(_BAYOS_UTIL_MEMORY_H_INCLUDED_)
#define _BAYOS_UTIL_MEMORY_H_INCLUDED_

namespace system
{
    /* メモリマネージャー */
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
// ここから下はメモリー操作用の関数
//

/* C言語の関数宣言 */
extern "C"
{
    void* memcpy(void* s1, const void* s2, size_t size);
};

//
// ここから下は C++言語の new / delete サポート
// および C言語の malloc / free サポート
//

extern Memory g_km;

void* operator new(size_t size);
void* operator new[](size_t size);
void  operator delete(void* address);
void  operator delete[](void* address);

/* C言語の関数宣言 */
extern "C"
{
    void* malloc(size_t size);
    void  free(void* address);
};

#endif
