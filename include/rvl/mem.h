#ifndef _RVL_MEM_H

typedef struct MEMAllocator;

typedef void* (*MEMAllocatorAllocFunc)(struct MEMAllocator* allocator, unsigned int size);
typedef void (*MEMAllocatorFreeFunc)(struct MEMAllocator* allocator, void* block);

typedef struct _MEMAllocatorFuncs {
    MEMAllocatorAllocFunc allocFunc;
    MEMAllocatorFreeFunc freeFunc;
} MEMAllocatorFuncs;

typedef struct _MEMAllocator {
    MEMAllocatorFuncs *funcs;
    void *heap;
    unsigned int heapParam1;
    unsigned int heapParam2;
} MEMAllocator;

#endif // _RVL_MEM_H
