#pragma once
#include_next <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wrap_FILE wrap_FILE;

wrap_FILE *wrap_fopen(const char *filename, const char *mode);
int wrap_fclose(wrap_FILE *file);

size_t wrap_fread(void *buffer, size_t size, size_t count, wrap_FILE *file);
int wrap_fgetc(wrap_FILE *file);

size_t wrap_fwrite(const void *buffer, size_t size, size_t count, wrap_FILE *file);

int wrap_fseek(wrap_FILE *file, long offset, int origin);
long wrap_ftell(wrap_FILE *file);

int wrap_feof(wrap_FILE *file);
int wrap_ferror(wrap_FILE *file);
int wrap_clearerr(wrap_FILE *file);

extern wrap_FILE *wrap_stdin;
extern wrap_FILE *wrap_stdout;
extern wrap_FILE *wrap_stderr;
#define stdin wrap_stdin
#define stdout wrap_stdout
#define stderr wrap_stderr

#define FILE wrap_FILE
#define fopen wrap_fopen
#define fclose wrap_fclose
#define fread wrap_fread
#define fgetc wrap_fgetc
#define getc fgetc
#define fwrite wrap_fwrite
#define fseek wrap_fseek
#define ftell wrap_ftell
#define feof wrap_feof
#define ferror wrap_ferror
#define clearerr wrap_clearerr

#ifdef __cplusplus
}
#endif

