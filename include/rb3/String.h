#ifndef _STRING_H
#define _STRING_H

typedef struct _String
{
    int *vtable;
    int length;
    char *buf;
} String;

#endif // _STRING_H