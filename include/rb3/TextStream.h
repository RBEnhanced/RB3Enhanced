#ifndef _TEXTSTREAM_H
#define _TEXTSTREAM_H

typedef struct _TextStream _TextStream;

typedef void (*TextStreamDestructor_t)(_TextStream *thisTextStream);
typedef void (*TextStreamPrint_t)(_TextStream *thisTextStream, char *string);

typedef struct _TextStream_vtable
{
    TextStreamDestructor_t destructor;
    TextStreamPrint_t print;
} TextStream_vtable;

typedef struct _TextStream
{
    TextStream_vtable *vtable;
} TextStream;

TextStream *TextStreamConstructor(TextStream *thisTextStream);

#endif // _TEXTSTREAM_H
