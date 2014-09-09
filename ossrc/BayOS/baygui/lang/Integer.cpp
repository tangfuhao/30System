#include "Integer.h"

/** �v���~�e�B�u�� int �̈�����\���A�V�������蓖�Ă�ꂽ Integer �I�u�W�F�N�g���\�z���܂��B*/
Integer::Integer(int value)
{
    this->value = value;
}

/** ������ŕ\������鐔�l��\���A�V�������蓖�Ă�ꂽ Integer �I�u�W�F�N�g�𐶐����܂��B*/
#if 0
Integer::Integer(const char* s)
{
    this->value = atoi(s);
}
#endif

/** ���̃I�u�W�F�N�g���w�肳�ꂽ�I�u�W�F�N�g�Ɣ�r���܂��B*/
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

/** C++ �ɂ��[�� instanceof */
bool Integer::instanceof(int classType)
{
    return Object::instanceof(classType) ||
           (bayos_lang_Integer == classType);
}

/** ���� Integer �̒l�� int �l�Ƃ��ĕԂ��܂��B*/
int Integer::intValue()
{
    return this->value;
}

/** ������̈����𕄍��t�� 10 �i���̐����^�Ƃ��ĉ�͂��܂��B*/
#if 0
int Integer::parseInt(const char* s)
{
    return atoi(s);
}
#endif
