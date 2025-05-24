#ifndef _CHUNKSTREAM_H
#define _CHUNKSTREAM_H

#include "rb3/BinStream.h"
#include "rb3/File.h"
#include "rb3/Platform.h"

typedef struct _ChunkStream ChunkStream;

typedef int (*ChunkStreamDestructor_t)(ChunkStream *thisChunkStream, int unk);
typedef void (*ChunkStreamFlush_t)(ChunkStream *thisChunkStream);
typedef int (*ChunkStreamTell_t)(ChunkStream *thisChunkStream);
typedef int (*ChunkStreamEOF_t)(ChunkStream *thisChunkStream);
typedef int (*ChunkStreamFail_t)(ChunkStream *thisChunkStream);
typedef char *(*ChunkStreamName_t)(ChunkStream *thisChunkStream);
typedef char *(*ChunkStreamReadImpl_t)(ChunkStream *thisChunkStream, void *data, int bytes);
typedef char *(*ChunkStreamWriteImpl_t)(ChunkStream *thisChunkStream, void *data, int bytes);
typedef char *(*ChunkStreamSeekImpl_t)(ChunkStream *thisChunkStream, int offset, SeekType seekType);
typedef int (*ChunkStreamReturnsZero_t)();

typedef struct _ChunkStream_vtable
{
    ChunkStreamDestructor_t destructor;
    ChunkStreamFlush_t flush;
    ChunkStreamTell_t tell;
    ChunkStreamEOF_t eof;
    ChunkStreamFail_t fail;
    ChunkStreamName_t name;
    ChunkStreamReturnsZero_t cached;
    ChunkStreamReturnsZero_t getPlatform;
    ChunkStreamReadImpl_t readImpl;
    ChunkStreamWriteImpl_t writeImpl;
    ChunkStreamSeekImpl_t seekImpl;
} ChunkStream_vtable;

struct _ChunkStream
{
    ChunkStream_vtable *vtable;
};

ChunkStream *ChunkStreamConstructor(ChunkStream *thisChunkStream, char *fileName, FileType fileType, int chunkSize, char compress, Platform platform, char cached);

#endif // _CHUNKSTREAM_H