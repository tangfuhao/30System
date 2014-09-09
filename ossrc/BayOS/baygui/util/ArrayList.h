#if !defined(_BAYGUI_UTIL_ARRAYLIST_H_INCLUDED_)
#define _BAYGUI_UTIL_ARRAYLIST_H_INCLUDED_

using namespace baygui::lang;

namespace baygui
{
    namespace util
    {
        /** List �C���^�t�F�[�X�̃T�C�Y�ύX�\�Ȕz��̎����ł��B*/
        class ArrayList : public Object
        {
        private:
            /** ���X�g�̃f�t�H���g�e�ʂł��B*/
            static const int defaultCapacity = 5;
            /** ���X�g���̗v�f�ꗗ�ł��B*/
            Object** _elements;
            /** ���X�g���̗v�f���ł��B*/
            int _size;
            /** ���X�g�̌��݂̗e�ʂł��B*/
            int _capacity;

        private:
            /** �v���C�x�[�g�ϐ��̏��������s���܂��B*/
            void initialize();

        public:
            /** �f�t�H���g�R���X�g���N�^ */
            ArrayList();

            /** �f�X�g���N�^ */
            virtual ~ArrayList();

            /** ���X�g�̎w�肳�ꂽ�ʒu�ɁA�w�肳�ꂽ�v�f��}�����܂��B*/
            virtual bool add(Object* o);

            /** ���X�g���炷�ׂĂ̗v�f���폜���܂��B*/
            virtual void clear();

            /** ���X�g�Ɏw��̗v�f������ꍇ�� true ��Ԃ��܂��B*/
            virtual bool contains(Object* elem);

            /** ���X�g���̎w�肳�ꂽ�ʒu�ɂ���v�f��Ԃ��܂��B*/
            virtual Object* get(int index);

            /** �w�肳�ꂽ�����Ɠ������e�̗v�f��擪���猟�����܂��B*/
            virtual int indexOf(Object* elem);

            /** C++ �ɂ��[�� instanceof */
            virtual bool instanceof(int classType);

            /** ���X�g�ɗv�f���Ȃ����ǂ����𔻒肵�܂��B*/
            virtual bool isEmpty();

            /** ���X�g���̎w�肳�ꂽ�ʒu����v�f���폜���܂��B*/
            virtual Object* remove(int index);

            /** ���X�g���ɂ���v�f�̐���Ԃ��܂��B*/
            virtual int size();

            /** ���X�g���̂��ׂĂ̗v�f�������������Ŋi�[����Ă���z���Ԃ��܂��B*/
            virtual Object** toArray();
        };
    }
}

#endif
