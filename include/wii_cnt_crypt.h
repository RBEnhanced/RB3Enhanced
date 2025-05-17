/*
    RB3Enhanced - wii_cnt_crypt.h
*/

#ifdef RB3E_WII

#include <stdint.h>

typedef struct _RB3E_CNTFileSD {
    int fd;
    uint32_t titleId;
    int contentLength;
    int startOffset;
    uint16_t contentIndex;
    struct AES_ctx *aesCtx;
    uint8_t *arcHeader;
    int lastBlockIndex;
    uint8_t lastBlock[0x10];
    uint8_t lastBlockEnc[0x10];
} RB3E_CNTFileSD;

RB3E_CNTFileSD *RB3E_OpenCNTFileSD(const char *filepath);
void RB3E_CloseCNTFileSD(RB3E_CNTFileSD *file);
void RB3E_CNTFileRead(RB3E_CNTFileSD *file, int offset, uint8_t *buffer, int length);

#endif // RB3E_WII
