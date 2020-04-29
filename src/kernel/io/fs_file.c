#include "fs_file.h"

#include "fs/fs.h"

File *FSFile_new(const char *path)
{
    FSEntry *data = getEntry(path);

    // Not found
    if (!data)
        return NULL;
    
    return File_new(data, FSFile_ops());
}

FileOps *FSFile_ops()
{
    FileOps *ops = malloc(sizeof(FileOps));

    *ops = (FileOps) {
        .read = FSFile_read,
        .write = FSFile_write,
        .close = FSFile_close,
    };

    return ops;
}

ssize_t FSFile_read(File *f, void *buffer, size_t count)
{
    return FSEntry_read(f->data, buffer, count);
}

ssize_t FSFile_write(File *f, void *buffer, size_t count)
{
    return FSEntry_write(f->data, buffer, count);
}

void FSFile_close(File *f)
{
    FSEntry_del(f->data);
}
