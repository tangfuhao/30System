#if !defined(_BAYGUI_LANG_INTEGER_H_INCLUDED_)
#define _BAYGUI_LANG_INTEGER_H_INCLUDED_

using namespace baygui::lang;

namespace baygui
{
    namespace lang
    {
        /** Integer �N���X�́A�v���~�e�B�u�^ int �̒l���I�u�W�F�N�g���Ƀ��b�v���܂��B*/
        class Integer : public Object
        {
        private:
            /** �v���~�e�B�u�^ */
            int value;

        public:
            /** �v���~�e�B�u�� int �̈�����\���A�V�������蓖�Ă�ꂽ Integer �I�u�W�F�N�g���\�z���܂��B*/
            Integer(int value);

            /** ������ŕ\������鐔�l��\���A�V�������蓖�Ă�ꂽ Integer �I�u�W�F�N�g�𐶐����܂��B*/
            #if 0
            Integer(const char* s);
            #endif

            /** ���̃I�u�W�F�N�g���w�肳�ꂽ�I�u�W�F�N�g�Ɣ�r���܂��B*/
            virtual bool equals(Object* obj);

            /** C++ �ɂ��[�� instanceof */
            virtual bool instanceof(int classType);

            /** ���� Integer �̒l�� int �l�Ƃ��ĕԂ��܂��B*/
            virtual int intValue();

            /** ������̈����𕄍��t�� 10 �i���̐����^�Ƃ��ĉ�͂��܂��B*/
            #if 0
            static int parseInt(const char* s);
            #endif
        };
    }
}

#endif
