#include "ArrayList.h"

/** プライベート変数の初期化を行います。*/
void ArrayList::initialize()
{
    this->_elements = new Object*[defaultCapacity];
    this->_size = 0;
    this->_capacity = defaultCapacity;
}

/** デフォルトコンストラクタ */
ArrayList::ArrayList()
{
    initialize();
}

/** デストラクタ */
~ArrayList::ArrayList()
{
    delete [] this->_elements;
}

/** リストの指定された位置に、指定された要素を挿入します。*/
bool ArrayList::add(Object* o)
{
    if (this->_size == this->_capacity)
    {
        /* 足りなくなったら容量を倍に増やす */
        this->_capacity <<= 1;
        Object** ptr = new Object*[this->_capacity];
        for (int i = 0; i < this->_size; i++)
        {
            ptr[i] = this->_elements[i];
        }
        delete [] this->_elements;
        this->_elements = ptr;
    }
    this->_elements[this->_size] = o;
    this->_size++;
    return true;
}

/** リストからすべての要素を削除します。*/
void ArrayList::clear()
{
    delete [] this->_elements;
    initialize();
}

/** リストに指定の要素がある場合に true を返します。*/
bool ArrayList::contains(Object* elem)
{
    return indexOf(elem) != -1;
}

/** リスト内の指定された位置にある要素を返します。*/
Object* ArrayList::get(int index)
{
    return (index < 0) ? NULL : this->_elements[index];
}

/** 指定された引数と同じ内容の要素を先頭から検索します。*/
int ArrayList::indexOf(Object* elem)
{
    for (int i = 0; i < this->_size; i++)
    {
        if (this->_elements[i]->equals(elem)) return i;
    }
    return -1;
}

/** C++ による擬似 instanceof */
bool ArrayList::instanceof(int classType)
{
    return Object::instanceof(classType) ||
           (bayos_util_ArrayList == classType);
}

/** リストに要素がないかどうかを判定します。*/
bool ArrayList::isEmpty()
{
    return this->_size == 0;
}

/** リスト内の指定された位置から要素を削除します。*/
Object* ArrayList::remove(int index)
{
    if (index < 0) return NULL;
    this->_size--;
    Object* obj = this->_elements[index];
    for (int i = index; i < this->_size; i++)
    {
        this->_elements[i] = this->_elements[i + 1];
    }
    return obj;
}

/** リスト内にある要素の数を返します。*/
int ArrayList::size()
{
    return this->_size;
}

/** リスト内のすべての要素が正しい順序で格納されている配列を返します。*/
Object** ArrayList::toArray()
{
    return this->_elements;
}
