/*
    RB3Enhanced - wii_cnt_crypt.c
    Handling for encrypted and unencrypted Wii CNT(, CNTSD, ARC) files
*/

#ifdef RB3E_WII

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <io/fat-sd.h>
#include <rvl/ipc.h>
#include "rvl/cnt.h"
#include "rvl/wad.h"
#include "rvl/tmd.h"
#include "rb3/Mem.h"
#include "aes.h"
#include "ports.h"
#include "rb3enhanced.h"
#include "wii_cnt_crypt.h"

#define PRINT_BLOCK(name, x) RB3E_DEBUG(name ": %08x%08x%08x%08x", *(uint32_t *)&x[0], *(uint32_t *)&x[4], *(uint32_t *)&x[8], *(uint32_t *)&x[12])

// to be filled in by the brainslug launcher
uint8_t RB3E_ConsolePRNGKey[0x10] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static uint8_t nullKey[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// This is really slow apparently
void IOS_AES_Decrypt(uint8_t *input, uint8_t *key, uint8_t *iv, uint8_t *output)
{
    static ios_fd_t aesFd = -1;
    if (aesFd == -1)
        aesFd = IOS_Open("/dev/aes", IPC_OPEN_NONE);
    
    ioctlv vec[4] __attribute__((align(32)));
    // input vector
    vec[0].data = input;
    vec[0].len = 0x10;
    vec[1].data = key;
    vec[1].len = 0x10;
    // output vector
    vec[2].data = output;
    vec[2].len = 0x10;
    vec[3].data = iv;
    vec[3].len = 0x10;
    // decrypt
    IOS_Ioctlv(aesFd, 0x3, 2, 2, vec);
}

uint64_t OSGetTime();
uint64_t timeSpentInAES = 0;
uint64_t timeSpentInSD = 0;
uint64_t timeSpentInSeek = 0;
void RB3E_CNTFileReadBlock(RB3E_CNTFileSD *file, int blockIndex)
{
    // if we're trying to read the same block we're on, don't do anything
    if (blockIndex == file->lastBlockIndex)
        return;
    
    uint64_t time = 0;
    if (file->aesCtx == NULL) // unencrypted reads
    {
        if (blockIndex == 0)
        {
            SD_seek(file->fd, file->startOffset, SEEK_SET);
        }
        else if (blockIndex != file->lastBlockIndex + 1)
        {
            RB3E_DEBUG("WARNING! CNTFileReadBlock had to seek back, this is gonna hurt", NULL);
            SD_seek(file->fd, file->startOffset + (blockIndex * 0x10), SEEK_SET);
        }
        SD_read(file->fd, file->lastBlock, 0x10);
        file->lastBlockIndex = blockIndex;
    }
    else // encrypted reads
    {
        if (blockIndex == 0)
        {
            time = OSGetTime();
            // block index 0 uses the IV derived from the content index
            memset(file->aesCtx->Iv, 0, 0x10);
            *(uint16_t *)file->aesCtx->Iv = file->contentIndex;
            SD_seek(file->fd, file->startOffset, SEEK_SET);
            timeSpentInSeek += OSGetTime() - time;
        }
        else if (blockIndex == file->lastBlockIndex + 1)
        {
            time = OSGetTime();
            // if we're reading in the next block, we don't need to seek backwards
            //memcpy(file->aesCtx->Iv, file->lastBlockEnc, 0x10);
            timeSpentInSeek += OSGetTime() - time;
        }
        else
        {
            time = OSGetTime();
            RB3E_DEBUG("WARNING! CNTFileReadBlock had to seek back, this is gonna hurt", NULL);
            // all other blocks use the IV with the previous block's ciphertext
            SD_seek(file->fd, file->startOffset + ((blockIndex - 1) * 0x10), SEEK_SET);
            SD_read(file->fd, file->aesCtx->Iv, 0x10);
            timeSpentInSeek += OSGetTime() - time;
        }
        time = OSGetTime();
        // read and decrypt the current block
        SD_read(file->fd, file->lastBlock, 0x10);
        timeSpentInSD += OSGetTime() - time;
        
        time = OSGetTime();
        //memcpy(file->lastBlockEnc, file->lastBlock, 0x10); // keep a copy of the encrypted last block to use as the next IV
        //memcpy(file->lastBlock, file->lastBlockEnc, 0x10);
        AES_CBC_decrypt_buffer(file->aesCtx, file->lastBlock, 0x10);
        timeSpentInAES += OSGetTime() - time;
        file->lastBlockIndex = blockIndex;
    }
}

void RB3E_CNTFileRead(RB3E_CNTFileSD *file, int offset, uint8_t *buffer, int length)
{
    int blockOffset = offset & 0xF;
    int curBlock = (offset & ~0xF) >> 4;
    int readBytes = 0;
    uint64_t timeStart = OSGetTime();
    timeSpentInAES = 0;
    timeSpentInSD = 0;
    timeSpentInSeek = 0;
    while (readBytes < length)
    {
        int toRead = 0x10 - blockOffset;
        if (length - readBytes < 0x10)
            toRead -= (toRead - (length - readBytes));
        if (toRead < 1 || toRead > 0x10)
        {
            RB3E_MSG("TRIED TO READ 0x%x BYTE BLOCK!", toRead);
            break;
        }
        RB3E_CNTFileReadBlock(file, curBlock);
        memcpy(buffer + readBytes, file->lastBlock + blockOffset, toRead);
        //RB3E_DEBUG("Wrote 0x%x to %p", toRead, buffer + readBytes);
        curBlock++;
        blockOffset = 0;
        readBytes += toRead;
    }
    RB3E_DEBUG("%p:0x%x:0x%x - %lli tb", buffer, offset, length, OSGetTime() - timeStart);
    RB3E_DEBUG("seek %lli sd %lli aes %lli", timeSpentInSeek, timeSpentInSD, timeSpentInAES);
}

void RB3E_CloseCNTFileSD(RB3E_CNTFileSD *file)
{
    if (file->fd != -1)
    {
        RB3E_CloseFile(file->fd);
        file->fd = -1;
    }
    if (file->aesCtx != NULL)
    {
        MemFree(file->aesCtx);
        file->aesCtx = NULL;
    }
    if (file->arcHeader != NULL)
    {
        MemFree(file->arcHeader);
        file->arcHeader = NULL;
    }
    MemFree(file);
}

static inline int alignoffset(int off, int align)
{
    return ((off + align - 1) & ~(align - 1));
}

static inline int getFlippedContent(uint8_t *contentFlags)
{
    int index = 0, i = 0, j = 0;
    for (i = 0; i < 0x40; i++)
    {
        for (j = 0; j < 8; j++)
        {
            unsigned char mask = (1 << j);
            if (contentFlags[i] == mask) goto found;
            index++;
        }
    }
found:
    return index;
}

RB3E_CNTFileSD *RB3E_OpenCNTFileSD(const char *filepath, unsigned long long titleid, unsigned int index)
{
    // open a handle to the file
    int fd = RB3E_OpenFile(filepath, 0);
    if (fd == -1)
    {
        RB3E_MSG("Failed to open CNT archive '%s'", filepath);
        return NULL;
    }
    // allocate a buffer for our file metadata
    RB3E_CNTFileSD *file = (RB3E_CNTFileSD *)MemAlloc(sizeof(RB3E_CNTFileSD), 0);
    if (file == NULL)
    {
        RB3E_MSG("Could not allocate enough memory for CNTSD file", NULL);
        RB3E_CloseFile(fd);
        return NULL;
    }
    memset(file, 0, sizeof(RB3E_CNTFileSD));
    file->fd = fd;
    file->lastBlockIndex = -1;
    // read the header of the file
    uint32_t magic[2];
    RB3E_ReadFile(fd, 0, magic, 8);
    if (magic[1] == BACKUP_WAD_MAGIC)
    {
        // we're an encrypted file, allocate an AES context
        file->aesCtx = MemAlloc(sizeof(struct AES_ctx), 0);
        if (file->aesCtx == NULL)
        {
            RB3E_MSG("Failed to allocate space for the AES context", NULL);
            RB3E_CloseCNTFileSD(file);
            return NULL;
        }
        // read the WAD header and get the current content index from it
        backup_wad_header_t wad;
        RB3E_ReadFile(fd, 0, &wad, sizeof(backup_wad_header_t));
        int index = getFlippedContent(wad.included_contents);
        RB3E_MSG("Host %.4s, TMD size 0x%x, Content size 0x%x", wad.title_id.name.code, wad.content_tmd_size, wad.content_data_size);
        if (index == 0 || index > 511)
        {
            RB3E_MSG("! Invalid index %i!", index);
            RB3E_CloseCNTFileSD(file);
            return NULL;
        }
        RB3E_DEBUG("Content index %i", index);
        file->contentIndex = index;
        // read the TMD to get the true content length
        tmd_header_t tmd;
        RB3E_ReadFile(fd, sizeof(backup_wad_header_t), &tmd, sizeof(tmd_header_t));
        if (tmd.num_contents < index)
        {
            RB3E_MSG("Invalid number of contents!", NULL);
            RB3E_CloseCNTFileSD(file);
            return NULL;
        }
        // i don't know if this is *always* true but i'm assuming it is true that content indices are always sequential
        tmd_content_t tmd_cnt;
        RB3E_ReadFile(fd, sizeof(backup_wad_header_t) + sizeof(tmd_header_t) + (index * sizeof(tmd_content_t)), &tmd_cnt, sizeof(tmd_content_t));
        if (tmd_cnt.index != index)
        {
            RB3E_MSG("Wrong TMD content index (tmd:%i != flipped:%i)", tmd_cnt.index, index);
            RB3E_CloseCNTFileSD(file);
            return NULL;
        }
        RB3E_MSG("%.4s/%03i - content: %08x, size: 0x%llx", tmd.title_id.name.code, index, tmd_cnt.id, tmd_cnt.size);
        file->titleId = *(uint32_t *)tmd.title_id.name.code;
        file->contentLength = tmd_cnt.size;
        // get the start of the encrypted data blob
        int startOfData = alignoffset(sizeof(backup_wad_header_t) + wad.content_tmd_size, 0x40);
        RB3E_DEBUG("Content starts at 0x%x", startOfData);
        file->startOffset = startOfData;
        // try to decrypt it with the NULL PRNG key (sZA-sZG)
        RB3E_DEBUG("Attempting to read with NULLed key", NULL);
        PRINT_BLOCK("Key", nullKey);
        AES_init_ctx(file->aesCtx, nullKey);
        memcpy(file->aesKey, nullKey, 0x10);
        RB3E_CNTFileReadBlock(file, 0);
        if (*(uint32_t *)file->lastBlock != ARC_MAGIC)
        {
            // that failed - try to decrypt it with the console-specific PRNG key (sZH+)
            RB3E_DEBUG("Trying console specific PRNG key", NULL);
            PRINT_BLOCK("Key", RB3E_ConsolePRNGKey);
            AES_init_ctx(file->aesCtx, RB3E_ConsolePRNGKey);
            memcpy(file->aesKey, RB3E_ConsolePRNGKey, 0x10);
            file->lastBlockIndex = -1;
            RB3E_CNTFileReadBlock(file, 0);
            if (*(uint32_t *)file->lastBlock != ARC_MAGIC)
            {
                // couldn't decrypt
                RB3E_MSG("Failed to decrypt the BIN file", NULL);
                PRINT_BLOCK("Last Block", file->lastBlock);
                RB3E_CloseCNTFileSD(file);
                return NULL;
            }
        }
        RB3E_MSG("Opened BIN successfully!", NULL);
        // read the ARC file header into RAM
        u8_header_t archeader;
        RB3E_CNTFileRead(file, 0, (uint8_t *)&archeader, sizeof(u8_header_t));
        // allocate the size of the header - data_offset doubles as the length of the full "header"
        void *fullarcheader = MemAlloc(archeader.data_offset, 0);
        if (fullarcheader == NULL)
        {
            RB3E_MSG("Failed to allocate space for the header", NULL);
            RB3E_CloseCNTFileSD(file);
            return NULL;
        }
        // read the header, including node and string tables
        RB3E_CNTFileRead(file, 0, fullarcheader, archeader.data_offset);
        file->arcHeader = fullarcheader;
        RB3E_DEBUG("Created file %p", file);
        return file;
    }
    else if (magic[0] == ARC_MAGIC)
    {
        // untested, just try to handle the ARC file directly
        file->aesCtx = NULL;
        file->contentIndex = index;
        file->contentLength = RB3E_FileSize(file->fd);
        file->startOffset = 0;
        file->titleId = (uint32_t)titleid;
        // read the ARC file header into RAM
        u8_header_t archeader;
        RB3E_CNTFileRead(file, 0, (uint8_t *)&archeader, sizeof(u8_header_t));
        // allocate the size of the header - data_offset doubles as the length of the full "header"
        void *fullarcheader = MemAlloc(archeader.data_offset, 0);
        if (fullarcheader == NULL) {
            RB3E_MSG("Failed to allocate space for the header", NULL);
            RB3E_CloseCNTFileSD(file);
            return NULL;
        }
        // read the header, including node and string tables
        RB3E_CNTFileRead(file, 0, fullarcheader, archeader.data_offset);
        file->arcHeader = fullarcheader;
        return file;
    }
    else
    {
        RB3E_MSG("'%s' was not valid CNT file.", filepath);
        RB3E_CloseCNTFileSD(file);
        return NULL;
    }
}

void ParseKeysFile(uint8_t *keys_data, size_t keys_len)
{
    RB3E_DEBUG("0x%x bytes key file", keys_len);
    // check if its a BootMii-style keys.bin
    if (keys_len >= 0x400 && *(uint32_t *)&keys_data[0x114] == 0xEBE42A22)
    {   
        memcpy(RB3E_ConsolePRNGKey, keys_data + 0x168, 0x10);
    }
    // check if its an OTP file dump
    else if (keys_len >= 0x100 && *(uint32_t *)&keys_data[0x14] == 0xEBE42A22)
    {
        memcpy(RB3E_ConsolePRNGKey, keys_data + 0x68, 0x10);
    }
    // check if its a straight up key
    else if (keys_len == 0x10)
    {
        memcpy(RB3E_ConsolePRNGKey, keys_data, 0x10);
    }
    else
    {
        RB3E_DEBUG("FAILED TO LOAD KEY!", NULL);
        return;
    }
    PRINT_BLOCK("Loaded Key", RB3E_ConsolePRNGKey);
}

void TryToLoadPRNGKeyFromFile()
{
    uint8_t keys_buffer[0x400]; // size of keys.bin file

    // check if the loader already loaded a prng key
    if (memcmp(RB3E_ConsolePRNGKey, nullKey, 0x10) != 0)
    {
        RB3E_DEBUG("PRNG key loaded, not scanning for file", NULL);
        return;
    }

    // DOLPHIN ONLY: try to load keys.bin from the NAND
    if (RB3E_IsEmulator())
    {
        ios_fd_t nandfd = IOS_Open("/keys.bin", IPC_OPEN_READ);
        if (nandfd >= 0)
        {
            RB3E_DEBUG("Loading keys from NAND", NULL);
            ios_ret_t readret = IOS_Read(nandfd, keys_buffer, sizeof(keys_buffer));
            IOS_Close(nandfd);
            if (readret >= IPC_OK)
            {
                ParseKeysFile(keys_buffer, readret);
            }
            return;
        }
    }

    // try a few files on SD
    // PRNG key as binary
    if (RB3E_FileExists("sd:/prng_key.bin"))
    {
        int keysfd = RB3E_OpenFile("sd:/prng_key.bin", 0);
        if (keysfd != -1)
        {
            RB3E_DEBUG("Loading keys from prng_key.bin", NULL);
            int r = RB3E_ReadFile(keysfd, 0, keys_buffer, 0x10);
            RB3E_CloseFile(keysfd);
            ParseKeysFile(keys_buffer, r == 0x10 ? 0x10 : 0);
            return;
        }
    }
    // vWii/xyzzy OTP backup
    if (RB3E_FileExists("sd:/otp.bin"))
    {
        int otpfd = RB3E_OpenFile("sd:/otp.bin", 0);
        if (otpfd != -1)
        {
            RB3E_DEBUG("Loading keys from otp.bin", NULL);
            int r = RB3E_ReadFile(otpfd, 0, keys_buffer, sizeof(keys_buffer));
            RB3E_CloseFile(otpfd);
            ParseKeysFile(keys_buffer, r);
            return;
        }
    }
    // BootMii/xyzzy keys.bin backup
    if (RB3E_FileExists("sd:/keys.bin"))
    {
        int keysfd = RB3E_OpenFile("sd:/keys.bin", 0);
        if (keysfd != -1)
        {
            RB3E_DEBUG("Loading keys from keys.bin", NULL);
            int r = RB3E_ReadFile(keysfd, 0, keys_buffer, sizeof(keys_buffer));
            RB3E_CloseFile(keysfd);
            ParseKeysFile(keys_buffer, r);
            return;
        }
    }
}

#endif // RB3E_WII
