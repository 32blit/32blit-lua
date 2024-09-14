#pragma once
#include_next <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wrap_FILE wrap_FILE;

wrap_FILE *wrap_fopen(const char *filename, const char *mode);
wrap_FILE *wrap_freopen(const char *filename, const char *mode, wrap_FILE *file);
int wrap_fclose(wrap_FILE *file);

int wrap_setvbuf(wrap_FILE *file, char *buffer, int mode, size_t size);

size_t wrap_fread(void *buffer, size_t size, size_t count, wrap_FILE *file);
int wrap_fgetc(wrap_FILE *file);
char *wrap_fgets(char *str, int count, wrap_FILE *file);
int wrap_ungetc(int ch, wrap_FILE *file);

size_t wrap_fwrite(const void *buffer, size_t size, size_t count, wrap_FILE *file);
int wrap_fprintf(wrap_FILE *file, const char *format, ...);

int wrap_fseek(wrap_FILE *file, long offset, int origin);
long wrap_ftell(wrap_FILE *file);

int wrap_feof(wrap_FILE *file);
int wrap_ferror(wrap_FILE *file);
int wrap_clearerr(wrap_FILE *file);
int wrap_fflush(wrap_FILE *file);

wrap_FILE *wrap_tmpfile();

extern wrap_FILE *wrap_stdin;
extern wrap_FILE *wrap_stdout;
extern wrap_FILE *wrap_stderr;
#define stdin wrap_stdin
#define stdout wrap_stdout
#define stderr wrap_stderr

#define FILE wrap_FILE
#define fopen wrap_fopen
#define freopen wrap_freopen
#define fclose wrap_fclose
#define setvbuf wrap_setvbuf
#define fread wrap_fread
#define fgetc wrap_fgetc
#define getc fgetc
#define fgets wrap_fgets
#define ungetc wrap_ungetc
#define fwrite wrap_fwrite
#define fprintf wrap_fprintf
#define fseek wrap_fseek
#define ftell wrap_ftell
#define feof wrap_feof
#define ferror wrap_ferror
#define clearerr wrap_clearerr
#define fflush wrap_fflush
#define tmpfile wrap_tmpfile

#ifdef __cplusplus
}
#endif

