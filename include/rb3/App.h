#ifndef _APP_H
#define _APP_H

extern void AppConstructor(void *thisApp, int argc, char **argv);
extern void *NewFile(char *fileName, int flags); // doesn't belong to a namespace?

#endif // _APP_H