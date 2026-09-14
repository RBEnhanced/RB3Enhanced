#ifndef _STRING_H
#define _STRING_H

typedef struct _String
{
    int *vtable;
    int length;
    char *buf;
} String;

int StringConstructor(int *thisFactory, String *str);

#endif // _STRING_H