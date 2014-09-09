#if !defined(_BAYGUI_LANG_OBJECT_H_INCLUDED_)
#define _BAYGUI_LANG_OBJECT_H_INCLUDED_

namespace baygui
{
    namespace lang
    {
        /** Object クラスは、クラス階層のルートです。すべてのクラスは、スーパークラスとして Object を持ちます。*/
        class Object
        {
        public:
            /** デフォルトコンストラクタ */
            Object();

            /** デストラクタ */
            virtual ~Object();

            /** このオブジェクトと他のオブジェクトが等しいかどうかを示します。*/
            virtual bool equals(Object* obj);

            /** C++ による擬似 instanceof */
            virtual bool instanceof(int classType);
        };
    }
}

#endif
