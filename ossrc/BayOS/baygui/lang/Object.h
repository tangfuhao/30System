#if !defined(_BAYGUI_LANG_OBJECT_H_INCLUDED_)
#define _BAYGUI_LANG_OBJECT_H_INCLUDED_

namespace baygui
{
    namespace lang
    {
        /** Object �N���X�́A�N���X�K�w�̃��[�g�ł��B���ׂẴN���X�́A�X�[�p�[�N���X�Ƃ��� Object �������܂��B*/
        class Object
        {
        public:
            /** �f�t�H���g�R���X�g���N�^ */
            Object();

            /** �f�X�g���N�^ */
            virtual ~Object();

            /** ���̃I�u�W�F�N�g�Ƒ��̃I�u�W�F�N�g�����������ǂ����������܂��B*/
            virtual bool equals(Object* obj);

            /** C++ �ɂ��[�� instanceof */
            virtual bool instanceof(int classType);
        };
    }
}

#endif
