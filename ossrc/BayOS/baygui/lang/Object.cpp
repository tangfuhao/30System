#include "Object.h"

/** �f�t�H���g�R���X�g���N�^ */
Object::Object()
{}

/** �f�X�g���N�^ */
~Object::Object()
{}

/** ���̃I�u�W�F�N�g�Ƒ��̃I�u�W�F�N�g�����������ǂ����������܂��B*/
bool Object::equals(Object* obj)
{
    return (this == obj);
}

/** C++ �ɂ��[�� instanceof */
bool Object::instanceof(int classType)
{
    return (bayos_lang_Object == classType);
}
