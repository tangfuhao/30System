#include "../bayos.h"

List<FileEntry*>* File::files = NULL;

/* �t�@�C���G���g���N���X�̔j�� */
FileEntry::~FileEntry()
{
    delete(name);
    delete(data);
}

/* �t�@�C���}�l�[�W���̏����� */
void File::init()
{
    File::files = new List<FileEntry*>();
    struct FileInfo* entries = (struct FileInfo *) (ADR_DISKIMG + 0x000200);
    for (int i = 0; i < 272; i++)
    {
        if (entries[i].size == -1) break;
        FileEntry* entry = new FileEntry();
        entry->size = entries[i].size;
        entry->addr = entries[i].addr;
        strcat(entry->name, (char *) entries[i].name);
        files->add(entry);
    }
}

/* �t�@�C���̓ǂݍ��� */
FileEntry* File::load(char* file_path)
{
    /* �t�@�C����������Ȃ����� */
    if (files->size() == 0)
    {
        return NULL;
    }

    for (int i = 0; i < files->size(); i++)
    {
        FileEntry* entry = files->get(i);
        if (strncmp(file_path, entry->name, strlen(entry->name)) == 0)
        {
            if (entry->data != NULL)
            {
                return entry;
            }
            else
            {
                byte* data = (byte *)(ADR_DISKIMG + entry->addr);
                int tek5s_size   = tek_checkformat(entry->size, data);
                if (tek5s_size <= 0)
                {
                    /* �ʏ�̃t�@�C�� */
                    entry->data = (byte *)malloc(entry->size);
                    memcpy(entry->data, data, entry->size);
                    return entry;
                }
                else
                {
                    /* tek5s�`�� */
                    entry->size = tek5s_size;
                    entry->data = (byte *)malloc(tek5s_size);
                    tek_decode(entry->size, data, entry->data);
                    return entry;
                }
            }
        }
    }

    /* �t�@�C����������Ȃ����� */
    return NULL;
}

/* �t�@�C���̈ꗗ��Ԃ� */
List<FileEntry*>* File::list()
{
    return File::files;
}
