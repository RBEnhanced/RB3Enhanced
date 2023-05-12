#ifndef _MEM_H
#define _MEM_H

extern void *MemAlloc(int size, int pool_maybe);
extern void MemFree(void *ptr);

#endif // _MEM_H
