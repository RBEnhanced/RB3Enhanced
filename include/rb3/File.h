#ifndef _FILE_H
#define _FILE_H

typedef enum _FileType
{
    kRead = 0,
    kWrite = 1,
    kReadNoArk = 2,
    kAppend = 3
} FileType;

// flags can be used to check the existence of files both inside and outside the ARK, similar to how the NewFile flags work
char FileExists(char *iFilename, int iMode);

#endif // _FILE_H