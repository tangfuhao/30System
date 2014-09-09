#if !defined(_BAYOS_DRIVER_FILE_H_INCLUDED_)
#define _BAYOS_DRIVER_FILE_H_INCLUDED_

/* C����̊֐��錾 */
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
    /* �t�@�C���G���g���N���X */
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

    /* �t�@�C���}�l�[�W���N���X */
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
