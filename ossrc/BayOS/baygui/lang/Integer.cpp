#include "Integer.h"

/** プリミティブな int の引数を表す、新しく割り当てられた Integer オブジェクトを構築します。*/
Integer::Integer(int value)
{
    this->value = value;
}

/** 文字列で表現される数値を表す、新しく割り当てられた Integer オブジェクトを生成します。*/
#if 0
Integer::Integer(const char* s)
{
    this->value = atoi(s);
}
#endif

/** このオブジェクトを指定されたオブジェクトと比較します。*/
bool Integer::equals(Object* obj)
{
    if (obj->instanceof(bayos_lang_Integer))
    {
        return this->intValue() == ((Integer *) obj)->intValue();
    }
    else
    {
        return false;
    }
}

/** C++ による擬似 instanceof */
bool Integer::instanceof(int classType)
{
    return Object::instanceof(classType) ||
           (bayos_lang_Integer == classType);
}

/** この Integer の値を int 値として返します。*/
int Integer::intValue()
{
    return this->value;
}

/** 文字列の引数を符号付き 10 進数の整数型として解析します。*/
#if 0
int Integer::parseInt(const char* s)
{
    return atoi(s);
}
#endif
