#ifndef _MEM_H
#define _MEM_H

#include "TextStream.h"

extern void *MemAlloc(int size, int pool_maybe);
extern void MemFree(void *ptr);

extern void MemPrintOverview(int type, TextStream *stream);
extern void MemPrint(int heap, TextStream *stream, char freeOnly);
extern int MemNumHeaps();

#endif // _MEM_H
