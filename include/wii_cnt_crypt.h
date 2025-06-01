/*
    RB3Enhanced - wii_cnt_crypt.h
*/

#ifdef RB3E_WII

#include <stdint.h>

typedef struct _RB3E_CNTFileSD {
    int fd; // 0x0
    uint32_t titleId; // 0x4
    int contentLength; // 0x8
    int startOffset; // 0xC
    uint16_t contentIndex; // 0x10
    uint16_t padding; // 0x12 - probably not needed
    struct AES_ctx *aesCtx; // 0x14
    uint8_t *arcHeader; // 0x18
    int lastBlockIndex; // 0x1c
    uint8_t aesKey[0x10]; // 0x20
    uint8_t lastBlock[0x10]; // 0x30
    uint8_t lastBlockEnc[0x10]; // 0x40
} RB3E_CNTFileSD;

RB3E_CNTFileSD *RB3E_OpenCNTFileSD(const char *filepath, unsigned long long titleid, unsigned int index);
void RB3E_CloseCNTFileSD(RB3E_CNTFileSD *file);
void RB3E_CNTFileRead(RB3E_CNTFileSD *file, int offset, uint8_t *buffer, int length);
void TryToLoadPRNGKeyFromFile();

#endif // RB3E_WII
