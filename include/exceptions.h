/*
    RB3Enhanced - exceptions.h
    Structures and function definitions for exception handling.
*/

#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <stdint.h>

#ifdef RB3E_XBOX
#include <xtl.h>
#endif

#ifdef RB3E_WII
typedef struct OSContext {
    uint32_t gprs[32];  // at 0x0
    uint32_t cr;        // at 0x80
    uint32_t lr;        // at 0x84
    uint32_t ctr;       // at 0x88
    uint32_t xer;       // at 0x8C
    double fprs[32];  // at 0x90
    uint32_t fpscr_pad; // at 0x190
    uint32_t fpscr;     // at 0x194
    uint32_t srr0;      // at 0x198
    uint32_t srr1;      // at 0x19C
    uint16_t mode;      // at 0x1A0
    uint16_t state;     // at 0x1A2
    uint32_t gqrs[8];   // at 0x1A4
    uint32_t psf_pad;   // at 0x1C4
    double psfs[32];  // at 0x1C8
} OSContext;
typedef struct _rb3e_exception_wii {
    uint32_t rb3e_base_address;
    uint8_t error;
    uint32_t dsisr;
    uint32_t dar;
    OSContext thread_ctx;
} rb3e_exception_wii;
#elif RB3E_XBOX
#define EXCEPTION_CONTEXT_SIZE 560
#endif

// '3EXx' where x is W for Wumbo, X for Xbox and P for PS3
#define EXCEPTIONPACK_HEADER_WII 0x33455857
#define EXCEPTIONPACK_HEADER_XBOX 0x33455858
#define EXCEPTIONPACK_HEADER_PS3 0x33455850

typedef struct _rb3e_exception_header {
    uint32_t magic;
    uint32_t version;
    char rb3e_buildtag[48];
    char rb3e_commit[48];
    uint16_t num_stackwalk;
    uint16_t num_memchunks;
} rb3e_exception_header;

typedef struct _rb3e_exception_memchunk {
    uint32_t address;
    uint32_t length;
} rb3e_exception_memchunk;

#endif // _EXCEPTIONS_H
