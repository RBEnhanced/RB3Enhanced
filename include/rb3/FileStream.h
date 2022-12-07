#ifndef _FILESTREAM_H
#define _FILESTREAM_H

typedef struct _FileStream FileStream;

typedef int (*FileStreamDestructor_t)(FileStream *thisFileStream, int unk);
typedef void (*FileStreamFlush_t)(FileStream *thisFileStream);
typedef int (*FileStreamTell_t)(FileStream *thisFileStream);
typedef int (*FileStreamEOF_t)(FileStream *thisFileStream);
typedef int (*FileStreamFail_t)(FileStream *thisFileStream);
typedef char *(*FileStreamName_t)(FileStream *thisFileStream);
typedef char *(*FileStreamReadImpl_t)(FileStream *thisFileStream, void *data, int bytes);
typedef char *(*FileStreamWriteImpl_t)(FileStream *thisFileStream, void *data, int bytes);
typedef char *(*FileStreamSeekImpl_t)(FileStream *thisFileStream, int offset, int seekType);
typedef int (*FileStreamReturnsZero_t)();

typedef struct _FileStream_vtable
{
    FileStreamDestructor_t destructor;
    FileStreamFlush_t flush;
    FileStreamTell_t tell;
    FileStreamEOF_t eof;
    FileStreamFail_t fail;
    FileStreamName_t name;
    FileStreamReturnsZero_t cached;
    FileStreamReturnsZero_t getPlatform;
    FileStreamReadImpl_t readImpl;
    FileStreamWriteImpl_t writeImpl;
    FileStreamSeekImpl_t seekImpl;
} FileStream_vtable;

struct _FileStream
{
    FileStream_vtable *vtable;
    char littleEndian;
    char unk[0x256];
};

FileStream *FileStreamConstructor(FileStream *thisFileStream, char *fileName, int fileType, char littleEndian);

#endif // _FILESTREAM_H