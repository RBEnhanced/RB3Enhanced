#ifndef _FILE_H
#define _FILE_H

// flags can be used to check the existence of files both inside and outside the ARK, similar to how the NewFile flags work
char FileExists(char *path, int flags);

#endif // _FILE_H