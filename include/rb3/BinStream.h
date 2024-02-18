#ifndef _BINSTREAM_H
#define _BINSTREAM_H

typedef enum _SeekType
{
    kSeekBegin = 0,
    kSeekCur = 1,
    kSeekEnd = 2,
} SeekType;

typedef struct _BinStream BinStream;

typedef void (*BinStreamDestructor_t)(BinStream *thisBinStream, int unk);
typedef char *(*BinStreamName_t)(BinStream *thisBinStream);
typedef int (*BinStreamReturnsZero_t)();
typedef void (*BinStreamPurecall_t)();

typedef struct _BinStream_vtable
{
    BinStreamDestructor_t destructor;
    BinStreamPurecall_t purecall1;
    BinStreamPurecall_t purecall2;
    BinStreamPurecall_t purecall3;
    BinStreamPurecall_t purecall4;
    BinStreamName_t name;
    BinStreamReturnsZero_t cached;
    BinStreamReturnsZero_t getPlatform;
    BinStreamPurecall_t purecall5;
    BinStreamPurecall_t purecall6;
    BinStreamPurecall_t purecall7;
} BinStream_vtable;

struct _BinStream
{
    BinStream_vtable *vtable;
};

#endif // _BINSTREAM_H