#include "Object.h"

/** デフォルトコンストラクタ */
Object::Object()
{}

/** デストラクタ */
~Object::Object()
{}

/** このオブジェクトと他のオブジェクトが等しいかどうかを示します。*/
bool Object::equals(Object* obj)
{
    return (this == obj);
}

/** C++ による擬似 instanceof */
bool Object::instanceof(int classType)
{
    return (bayos_lang_Object == classType);
}
