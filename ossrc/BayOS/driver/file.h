#if !defined(_BAYOS_DRIVER_FILE_H_INCLUDED_)
#define _BAYOS_DRIVER_FILE_H_INCLUDED_

/* C言語の関数宣言 */
extern "C"
{
    struct FileInfo
    {
        int size, addr;
        byte name[24];
    };
}

namespace driver
{
    /* ファイルエントリクラス */
    class FileEntry
    {
    public:
        int   size;
        int   addr;
        char  name[24];
        byte* data;

    public:
        ~FileEntry();
    };

    /* ファイルマネージャクラス */
    class File
    {
    public:
        static List<FileEntry*>* files;

    public:
        static void init();
        static FileEntry* load(char* file_path);
        static List<FileEntry*>* list();
    };
}

#endif
