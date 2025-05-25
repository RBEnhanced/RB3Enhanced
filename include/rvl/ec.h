#ifndef _RVL_EC_H
#define _RVL_EC_H

typedef struct _ec_content_info_t {
    unsigned int flags;
    unsigned short index;
    unsigned short type;
    unsigned long long size;
} ec_content_info_t;

typedef struct _ec_title_info_t {
    unsigned long long titleId;
    int isTmdPresent;
    int isOnDevice;
    unsigned int type;
    unsigned int version;
    unsigned int occupiedUserBlocks;
    unsigned int occupiedUserInodes;
    unsigned int occupiedSysBlocks;
    unsigned int occupiedSysInodes;
} ec_title_info_t;

int EC_GetContentInfos(unsigned long long titleId, ec_content_info_t *contentInfos, int *numContentInfos);
int EC_DeleteContents(unsigned long long titleId, unsigned short *contents, int numContents);

#endif // _RVL_EC_H
