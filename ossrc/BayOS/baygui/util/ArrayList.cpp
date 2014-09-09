#include "ArrayList.h"

/** �v���C�x�[�g�ϐ��̏��������s���܂��B*/
void ArrayList::initialize()
{
    this->_elements = new Object*[defaultCapacity];
    this->_size = 0;
    this->_capacity = defaultCapacity;
}

/** �f�t�H���g�R���X�g���N�^ */
ArrayList::ArrayList()
{
    initialize();
}

/** �f�X�g���N�^ */
~ArrayList::ArrayList()
{
    delete [] this->_elements;
}

/** ���X�g�̎w�肳�ꂽ�ʒu�ɁA�w�肳�ꂽ�v�f��}�����܂��B*/
bool ArrayList::add(Object* o)
{
    if (this->_size == this->_capacity)
    {
        /* ����Ȃ��Ȃ�����e�ʂ�{�ɑ��₷ */
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

/** ���X�g���炷�ׂĂ̗v�f���폜���܂��B*/
void ArrayList::clear()
{
    delete [] this->_elements;
    initialize();
}

/** ���X�g�Ɏw��̗v�f������ꍇ�� true ��Ԃ��܂��B*/
bool ArrayList::contains(Object* elem)
{
    return indexOf(elem) != -1;
}

/** ���X�g���̎w�肳�ꂽ�ʒu�ɂ���v�f��Ԃ��܂��B*/
Object* ArrayList::get(int index)
{
    return (index < 0) ? NULL : this->_elements[index];
}

/** �w�肳�ꂽ�����Ɠ������e�̗v�f��擪���猟�����܂��B*/
int ArrayList::indexOf(Object* elem)
{
    for (int i = 0; i < this->_size; i++)
    {
        if (this->_elements[i]->equals(elem)) return i;
    }
    return -1;
}

/** C++ �ɂ��[�� instanceof */
bool ArrayList::instanceof(int classType)
{
    return Object::instanceof(classType) ||
           (bayos_util_ArrayList == classType);
}

/** ���X�g�ɗv�f���Ȃ����ǂ����𔻒肵�܂��B*/
bool ArrayList::isEmpty()
{
    return this->_size == 0;
}

/** ���X�g���̎w�肳�ꂽ�ʒu����v�f���폜���܂��B*/
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

/** ���X�g���ɂ���v�f�̐���Ԃ��܂��B*/
int ArrayList::size()
{
    return this->_size;
}

/** ���X�g���̂��ׂĂ̗v�f�������������Ŋi�[����Ă���z���Ԃ��܂��B*/
Object** ArrayList::toArray()
{
    return this->_elements;
}
