#include "fs_file.h"

#include <k/io.h>
#include <string.h>
#include "drivers/hdd.h"

File *FSFile_new(const char *path, u8 mode)
{
    FSEntry *file = getEntry(path);

    // Not found
    if (!file)
    {
        if ((mode & F_WRITE) != 0 || (mode & F_APPEND) != 0)
        {
            char *parent = strdup(path);
            char *name = inplaceCutPath(parent);

            FSEntry *dir = getEntry(parent);

            // Can't create
            if (dir == NULL)
            {
                free(parent);
                return NULL;
            }

            // Create it
            file = FSEntry_touch(dir, name, 0);

            free(parent);
        }
        else
            return NULL;
    }

    FSFileData *data = malloc(sizeof(FSFileData));
    data->file = file;

    if ((mode & F_APPEND) != 0 || (mode & F_READ) != 0)
    {
        // Read the file into the buffer
        void *buffer = malloc(file->size);
        FSEntry_read(file, buffer, file->size);

        data->buf = Buffer_newFromBuffer(4 * HDD_SECTOR_SIZE, buffer, file->size);

        // Set the head to the end to append
        if ((mode & F_APPEND) != 0)
            data->buf->head = file->size;

        free(buffer);
    }
    else
        // Empty buffer
        data->buf = Buffer_new(4 * HDD_SECTOR_SIZE);

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
    size_t result = Buffer_read(((FSFileData*) f->data)->buf, buffer, count);

    // Don't wait
    return result == 0 ? -1 : result;
}

ssize_t FSFile_write(File *f, void *buffer, size_t count)
{
    if (count == 0)
        return -1;

    size_t result = Buffer_write(((FSFileData*) f->data)->buf, buffer, count);

    // Don't wait
    return result == 0 ? -1 : result;
}

void FSFile_close(File *f)
{
    // Write to the file its content
    FSEntry_write(((FSFileData*) f->data)->file, ((FSFileData*) f->data)->buf->data, ((FSFileData*) f->data)->buf->size);

    // Delete
    Buffer_del(((FSFileData*) f->data)->buf);
    FSEntry_del(((FSFileData*) f->data)->file);
    free(f->data);
}
