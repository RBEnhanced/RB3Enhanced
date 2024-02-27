/*
    RB3Enhanced - _LOv2_LOibc.c
    Definitions used by the compiler to use the OS's copy of libc.
    This file should not be included by any other file, to avoid conflicts.
*/

#ifdef RB3E_PS3

#include <sys/types.h>
#include <ctype.h>

extern int _sys_memcmp(const void *, const void *, size_t);
int memcmp(const void *b1, const void *b2, size_t s)
{
    return _sys_memcmp(b1, b2, s);
}

void *_sys_memcpy(void *, const void *, size_t);
void *memcpy(void *dst, const void *src, size_t s)
{
    return _sys_memcpy(dst, src, s);
}

void *_sys_memset(void *, int, size_t);
void *memset(void *dst, int c, size_t s)
{
    return _sys_memset(dst, c, s);
}

char *_sys_strcat(char *, const char *);
char *strcat(char *dst, const char *src)
{
    return _sys_strcat(dst, src);
}

int _sys_strcmp(const char *, const char *);
int strcmp(const char *s1, const char *s2)
{
    return _sys_strcmp(s1, s2);
}

char *_sys_strcpy(char *, const char *);
char *strcpy(char *dst, const char *src)
{
    return _sys_strcpy(dst, src);
}

size_t _sys_strlen(const char *);
size_t strlen(const char *s)
{
    return _sys_strlen(s);
}

void *_sys_memmove(void *, const void *, size_t);
void *memmove(void *dst, const void *src, size_t s)
{
    return _sys_memmove(dst, src, s);
}

char *_sys_strncat(char *, const char *, size_t);
char *strncat(char *dst, const char *src, size_t s)
{
    return _sys_strncat(dst, src, s);
}

int _sys_strncmp(const char *, const char *, size_t);
int strncmp(const char *s1, const char *s2, size_t s)
{
    return _sys_strncmp(s1, s2, s);
}

char *_sys_strncpy(char *, const char *, size_t);
char *strncpy(char *dst, const char *src, size_t s)
{
    return _sys_strncpy(dst, src, s);
}

char *_sys_strchr(const char *, int);
char *strchr(const char *s, int c)
{
    return _sys_strchr(s, c);
}

#define _CNTYPE_DATA_0_127                                                          \
    _CN, _CN, _CN, _CN, _CN, _CN, _CN, _CN,                                         \
        _CN, _CN | _SP, _CN | _SP, _CN | _SP, _CN | _SP, _CN | _SP, _CN, _CN,       \
        _CN, _CN, _CN, _CN, _CN, _CN, _CN, _CN,                                     \
        _CN, _CN, _CN, _CN, _CN, _CN, _CN, _CN,                                     \
        _SP | _BB, _PU, _PU, _PU, _PU, _PU, _PU, _PU,                               \
        _PU, _PU, _PU, _PU, _PU, _PU, _PU, _PU,                                     \
        _DI, _DI, _DI, _DI, _DI, _DI, _DI, _DI,                                     \
        _DI, _DI, _PU, _PU, _PU, _PU, _PU, _PU,                                     \
        _PU, _UP | _XD, _UP | _XD, _UP | _XD, _UP | _XD, _UP | _XD, _UP | _XD, _UP, \
        _UP, _UP, _UP, _UP, _UP, _UP, _UP, _UP,                                     \
        _UP, _UP, _UP, _UP, _UP, _UP, _UP, _UP,                                     \
        _UP, _UP, _UP, _PU, _PU, _PU, _PU, _PU,                                     \
        _PU, _LO | _XD, _LO | _XD, _LO | _XD, _LO | _XD, _LO | _XD, _LO | _XD, _LO, \
        _LO, _LO, _LO, _LO, _LO, _LO, _LO, _LO,                                     \
        _LO, _LO, _LO, _LO, _LO, _LO, _LO, _LO,                                     \
        _LO, _LO, _LO, _PU, _PU, _PU, _PU, _CN
#define _CNTYPE_DATA_128_255    \
    0, 0, 0, 0, 0, 0, 0, 0,     \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0

short _ctype_b[128 + 256] = {
    _CNTYPE_DATA_128_255,
    _CNTYPE_DATA_0_127,
    _CNTYPE_DATA_128_255,
};

const short *__ctype_ptr = (short *)_ctype_b + 128;

#endif // RB3E_PUS3
