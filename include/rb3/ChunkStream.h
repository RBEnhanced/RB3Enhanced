#ifndef _CHUNKSTREAM_H
#define _CHUNKSTREAM_H

typedef struct _ChunkStream ChunkStream;

typedef int (*ChunkStreamDestructor_t)(ChunkStream *thisChunkStream, int unk);
typedef void (*ChunkStreamFlush_t)(ChunkStream *thisChunkStream);
typedef int (*ChunkStreamTell_t)(ChunkStream *thisChunkStream);
typedef int (*ChunkStreamEOF_t)(ChunkStream *thisChunkStream);
typedef int (*ChunkStreamFail_t)(ChunkStream *thisChunkStream);
typedef char *(*ChunkStreamName_t)(ChunkStream *thisChunkStream);
typedef char *(*ChunkStreamReadImpl_t)(ChunkStream *thisChunkStream, void *unk, int unk2);
typedef char *(*ChunkStreamWriteImpl_t)(ChunkStream *thisChunkStream, void *unk, int unk2);
typedef char *(*ChunkStreamSeekImpl_t)(ChunkStream *thisChunkStream, int unk, int seekType);
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

ChunkStream *ChunkStreamConstructor(ChunkStream *thisChunkStream, char *fileName, int fileType, int flags, char unk2, int platform, char unk3);

#endif // _CHUNKSTREAM_H