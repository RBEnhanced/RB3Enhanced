/*
    RB3Enhanced - wii_cnt_hooks.c
    Hooks to CNT and EC functions to allow for custom content loading.
*/

#ifdef RB3E_WII

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <io/fat-sd.h>
#include "rvl/cnt.h"
#include "rvl/ec.h"
#include "rb3enhanced.h"
#include "utilities.h"
#include "ports.h"
#include "ppcasm.h"
#include "wii_cnt_crypt.h"

int contentInitHandleTitleNAND_hook(unsigned long long title_id, unsigned int content_index, cnt_handle *handle, MEMAllocator *allocator)
{
    // make a file path of the SD DLC file
    char titleid[8];
    char sdfilepath[256];
    memcpy(titleid, &title_id, 8);
    sprintf(sdfilepath, "sd:/private/wii/data/%.4s/%03i.bin", &titleid[4], content_index);
    // if the file exists then we're going to be opening that
    if (RB3E_FileExists(sdfilepath))
    {
        // open the bin file on the SD
        RB3E_CNTFileSD *cntfilesd = RB3E_OpenCNTFileSD(sdfilepath, title_id, content_index);
        if (cntfilesd == NULL) {
            return -1; // TODO: what's the proper CNT error for "file not found"?
        }
        // open an ARC handle
        arc_handle_t archandle;
        ARCInitHandle(cntfilesd->arcHeader, &archandle);
        // fill our output struct
        memcpy(&handle->nand.ArcHandle, &archandle, sizeof(arc_handle_t));
        handle->nand.FileDescriptor = -0xBEEF; // dude is hungry
        handle->nand.allocator = allocator;
        handle->type = cnthandle_nand;
        // sneak the handle to our RB3E_CNTFileSD in there
        handle->nand.ArcHandle.header->reserved[0] = (unsigned int)cntfilesd;
        // return success
        return 0;
    }
    // fall back to reading from NAND
    return contentInitHandleTitleNAND(title_id, content_index, handle, allocator);
}

int CNTReleaseHandle_hook(cnt_handle *handle)
{
    // check if the file is reading from NAND and has our fake beef fd
    if (handle->type == cnthandle_nand && handle->nand.FileDescriptor == -0xBEEF)
    {
        // get out the handle to our RB3E_CNTFileSD
        RB3E_CNTFileSD *cntfilesd = (RB3E_CNTFileSD *)handle->nand.ArcHandle.header->reserved[0];
        RB3E_DEBUG("Freeing %p", cntfilesd);
        RB3E_CloseCNTFileSD(cntfilesd);
        // we don't call the original release function as we manage our own memory
        return 0;
    }
    // fall back to the original release function
    return CNTReleaseHandle(handle);
}

int CNTRead_hook(cnt_file_info *file_info, void *buffer, int size)
{
    // check if the file is reading from NAND and has our fake beef fd
    if (file_info->type == cnthandle_nand && file_info->nand.CntHandle->FileDescriptor == -0xBEEF)
    {
        //RB3E_DEBUG("File read %p 0x%x", buffer, size);
        RB3E_CNTFileSD *cntfilesd = (RB3E_CNTFileSD *)file_info->nand.CntHandle->ArcHandle.header->reserved[0];
        RB3E_CNTFileRead(cntfilesd, file_info->nand.startoffset + file_info->nand.readOffset, buffer, size);
        file_info->nand.readOffset += size;
        return size;
    }
    // fall back to the original read function
    return CNTRead(file_info, buffer, size);
}

int EC_GetContentInfos_hook(unsigned long long titleId, ec_content_info_t *contentInfos, int *numContentInfos)
{
    // make a file path of the SD DLC folder
    char titleid[8];
    char sddirpath[256];
    memcpy(titleid, &titleId, 8);
    sprintf(sddirpath, "sd:/private/wii/data/%.4s/", &titleid[4]);
    if (numContentInfos != NULL)
    {
        int originalNumContentInfos = *numContentInfos;
        int r = 0;
        DIR_STATE_STRUCT ds;
        if (SD_diropen(&ds, sddirpath) != NULL)
        {
            RB3E_DEBUG("Enumerating %s", sddirpath);
            // call the original so information about files on the NAND are populated
            EC_GetContentInfos(titleId, contentInfos, numContentInfos);
            *numContentInfos = originalNumContentInfos;
            // we *do* have a directory, check if we need to fill the contentInfos buffer
            if (contentInfos != NULL)
            {
                char filename[MAX_FILENAME_LENGTH];
                struct stat fs;
                while (true)
                {
                    if (SD_dirnext(&ds, filename, &fs) == 0)
                    {
                        // hacky check to see if the filename is some form of "000.BIN"
                        if ((filename[0] >= '0' && filename[0] <= '9') &&
                            (filename[1] >= '0' && filename[1] <= '9') &&
                            (filename[2] >= '0' && filename[2] <= '9') &&
                            (filename[3] == '.') &&
                            (filename[4] & ~0x20) == 'B' &&
                            (filename[5] & ~0x20) == 'I' &&
                            (filename[6] & ~0x20) == 'N')
                        {
                            // make a content index out of the filename
                            int contentIndex =
                                ((filename[0] - '0') * 100) + ((filename[1] - '0') * 10) + (filename[2] - '0');
                            if (contentIndex < originalNumContentInfos)
                            {
                                // mark that DLC as installed
                                contentInfos[contentIndex].flags = 3;
                                contentInfos[contentIndex].index = contentIndex;
                                contentInfos[contentIndex].type = 0x4001; // DLC
                                // the filesize of the DLC is checked to determine if something is a song or not
                                contentInfos[contentIndex].size = fs.st_size;
                            }
                            else
                            {
                                RB3E_DEBUG("%i is out of range of %i", contentIndex, originalNumContentInfos);
                            }
                        }
                    }
                    else break;
                }
                return r;
            }
            else
            {
                *numContentInfos = 512; // act like we have every possible content index
                RB3E_DEBUG("Returning %i", *numContentInfos);
                SD_dirclose(&ds);
            }
            return r;
        }
        else
        {
            RB3E_DEBUG("No SD DLC for %.4s", &titleid[4]);
            // call the original function, we don't have our own directory
            return EC_GetContentInfos(titleId, contentInfos, numContentInfos);
        }
        return r;
    }
    else
    {
        return EC_GetContentInfos(titleId, contentInfos, numContentInfos);
    }
}

void InitCNTHooks()
{
    POKE_32(PORT_SDMODECHECK, LI(3, 3));
    POKE_32(PORT_SDMODECHECK + 4, BLR);
    POKE_B(&ARCInitHandle, PORT_ARCINITHANDLE);
    HookFunction(PORT_CONTENTINITHANDLETITLENAND, contentInitHandleTitleNAND, contentInitHandleTitleNAND_hook);
    HookFunction(PORT_CNTRELEASEHANDLE, CNTReleaseHandle, CNTReleaseHandle_hook);
    HookFunction(PORT_CNTREAD, CNTRead, CNTRead_hook);
    HookFunction(PORT_ECGETCONTENTINFOS, EC_GetContentInfos, EC_GetContentInfos_hook);
}

#endif // RB3E_WII
