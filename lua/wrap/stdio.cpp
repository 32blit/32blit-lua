#ifdef TARGET_32BLIT_HW

#include <stdio.h>

#include "engine/file.hpp"

// wrap stdio funcs around blit:: funcs
wrap_FILE *wrap_stdin = nullptr;
wrap_FILE *wrap_stdout = nullptr;
wrap_FILE *wrap_stderr = nullptr;

struct wrap_FILE
{
    blit::File file;
    uint32_t offset;

    uint8_t getc_buffer[512];
    int getc_buf_len, getc_buf_off;
};

wrap_FILE *wrap_fopen(const char *filename, const char *mode)
{
    int blit_mode;

    // no append mode
    if(mode[0] != 'r' && mode[0] != 'w')
        return nullptr;

    // ignore b flag
    int i = 1;
    if(mode[i] == 'b')
        i++;

    if(mode[0] == 'r')
    {
        if(mode[i] != 0) return nullptr; // no r+

        blit_mode = blit::OpenMode::read;
    }
    else if(mode[0] == 'w')
    {
        blit_mode = blit::OpenMode::write;
        if(mode[i] == '+')
            blit_mode |= blit::OpenMode::read;
    }

    auto ret = new wrap_FILE;
    ret->file.open(filename, blit_mode);
    ret->offset = 0;

    ret->getc_buf_len = ret->getc_buf_off = 0;

    if(!ret->file.is_open())
    {
        delete ret;
        return nullptr;
    }
    
    return ret;
}

int wrap_fclose(wrap_FILE *file)
{
    file->file.close();
    delete file;
    return 0;
}

size_t wrap_fread(void *buffer, size_t size, size_t count, wrap_FILE *file)
{
    if(!file) return 0;

    // invalidate getc buffer
    if(file->getc_buf_len)
    {
        file->offset += file->getc_buf_off;
        file->getc_buf_off = file->getc_buf_len = 0;
    }

    auto ret = file->file.read(file->offset, size * count, (char *)buffer);
    file->offset += ret;

    return ret < 0 ? 0 : ret / size;
}

int wrap_fgetc(wrap_FILE *file)
{
    if(!file) return 0;

    // refill getc buffer
    if(file->getc_buf_off >= file->getc_buf_len)
    {
        file->offset += file->getc_buf_off;
        file->getc_buf_len = file->file.read(file->offset, 512, (char*)file->getc_buffer);
        file->getc_buf_off = 0;
    }

    if(file->getc_buf_len)
        return file->getc_buffer[file->getc_buf_off++];

    return EOF;
}

size_t wrap_fwrite(const void *buffer, size_t size, size_t count, wrap_FILE *file)
{
    if(!file) return 0;

    // invalidate getc buffer
    if(file->getc_buf_len)
    {
        file->offset += file->getc_buf_off;
        file->getc_buf_off = file->getc_buf_len = 0;
    }

    auto ret = file->file.write(file->offset, size * count, (char *)buffer);
    file->offset += ret;

    return ret < 0 ? 0 : ret / size;
}


int wrap_fseek(wrap_FILE *file, long offset, int origin)
{
    if(!file) return 0;

    // invalidate getc buffer
    if(file->getc_buf_len)
    {
        file->offset += file->getc_buf_off;
        file->getc_buf_off = file->getc_buf_len = 0;
    }

    if(origin == SEEK_SET)
        file->offset = offset;
    else if(origin == SEEK_CUR)
        file->offset += offset;
    else
        file->offset = file->file.get_length() - offset;

    return 0;
}

long wrap_ftell(wrap_FILE *file)
{
    if(!file) return 0;

    return file->offset + file->getc_buf_off;
}

int wrap_feof(wrap_FILE *file)
{
    return file->offset == file->file.get_length();
}

int wrap_ferror(wrap_FILE *file)
{
    return 0;
}

int wrap_clearerr(wrap_FILE *file)
{
    return 0;
}

#endif