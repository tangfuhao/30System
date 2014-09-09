#if !defined(_BAYGUI_LANG_INTEGER_H_INCLUDED_)
#define _BAYGUI_LANG_INTEGER_H_INCLUDED_

using namespace baygui::lang;

namespace baygui
{
    namespace lang
    {
        /** Integer クラスは、プリミティブ型 int の値をオブジェクト内にラップします。*/
        class Integer : public Object
        {
        private:
            /** プリミティブ型 */
            int value;

        public:
            /** プリミティブな int の引数を表す、新しく割り当てられた Integer オブジェクトを構築します。*/
            Integer(int value);

            /** 文字列で表現される数値を表す、新しく割り当てられた Integer オブジェクトを生成します。*/
            #if 0
            Integer(const char* s);
            #endif

            /** このオブジェクトを指定されたオブジェクトと比較します。*/
            virtual bool equals(Object* obj);

            /** C++ による擬似 instanceof */
            virtual bool instanceof(int classType);

            /** この Integer の値を int 値として返します。*/
            virtual int intValue();

            /** 文字列の引数を符号付き 10 進数の整数型として解析します。*/
            #if 0
            static int parseInt(const char* s);
            #endif
        };
    }
}

#endif
