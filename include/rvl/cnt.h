#ifndef _RVL_CNT_H
#define _RVL_CNT_H

#include <stdint.h>
#include <rvl/mem.h>
#include "emvolution/dvd.h"

// -- ARC/U8, on-disk --

// 'U.8-'
#define ARC_MAGIC 0x55AA382D

// https://wiki.tockdom.com/wiki/U8_(File_Format)
// header of ARC/U8 files
typedef struct _u8_header_t {
    unsigned int magic;
    int first_node;
    int nodes_size;
    int data_offset;
    unsigned int reserved[4];
} u8_header_t; // 0x20

// the types of nodes within an ARC
typedef enum _u8_node_type_t {
    arcnode_file = 0,
    arcnode_dir = 1
} u8_node_type_t;

// info about a file node
typedef struct _u8_node_file_t {
    int data_offset;
    int data_size;
} u8_node_file_t;
// info about a directory node
typedef struct _u8_node_dir_t {
    int parent_node_index;
    int next_node;
} u8_node_dir_t;

// an ARC node itself
typedef struct _u8_node_t {
    int node_type : 8;
    int string_offset : 24;
    union {
        u8_node_file_t file;
        u8_node_dir_t dir;
    };
} u8_node_t; // 0xC

// -- ARC, internally --

typedef struct _arc_handle_t {
    u8_header_t *header;
    u8_node_t *nodes;
    uint8_t *file;
    unsigned int count;
    const char *strings;
    unsigned int fstSize;
    int entrynum;
} arc_handle_t;

typedef struct _arc_file_info_t {
    arc_handle_t *handle;
    unsigned int offset;
    unsigned int size;
} arc_file_info_t;

typedef struct _arc_entry_t {
    arc_handle_t *handle;
    unsigned int path;
    int type;
    const char *name;
} arc_entry_t;

// -- CNT, internally --

typedef enum _cnt_handle_type {
    cnthandle_nand = 1,
    cnthandle_dvd = 2
} cnt_handle_type;

typedef struct _cnt_handle_nand {
    arc_handle_t ArcHandle;
    long FileDescriptor;
    void *allocator;
} cnt_handle_nand;

typedef struct _cnt_handle_dvd {
    unsigned char backing[0x24];
} cnt_handle_dvd;

typedef struct _cnt_handle {
    union {
        cnt_handle_nand nand;
        cnt_handle_dvd dvd;
    };
    unsigned char type;
} cnt_handle;

typedef struct _cnt_dir {
    unsigned char backing[0x10];
    unsigned char type;
} cnt_dir;

typedef struct _cnt_dir_entry {
    union {
        arc_entry_t arc;
    };
    unsigned char type;
} cnt_dir_entry;

typedef struct _cnt_file_info_nand {
    cnt_handle_nand *CntHandle;
    unsigned int startoffset;
    unsigned int length;
    long readOffset;
} cnt_file_info_nand;

typedef struct _cnt_file_info_dvd {
    DVDFileInfo fileInfo;
    long readOffset;
} cnt_file_info_dvd;

typedef struct _cnt_file_info {
    union {
        cnt_file_info_nand nand;
        cnt_file_info_dvd dvd;
    };
    unsigned char type;
} cnt_file_info;

int contentInitHandleTitleNAND(unsigned long long title_id, unsigned int content_index, cnt_handle *handle, MEMAllocator *allocator);
int CNTReleaseHandle(cnt_handle *handle);

int CNTOpen(cnt_handle *handle, const char *path, cnt_file_info *file_info);
int CNTGetLength(cnt_file_info *file_info);
int CNTClose(cnt_file_info *file_info);
int CNTRead(cnt_file_info *file_info, void *buffer, int length);
int CNTSeek(cnt_file_info *file_info, int position, int seek_from);

int CNTOpenDir(cnt_handle *handle, const char *path, cnt_dir *dir);
int CNTReadDir(cnt_dir *dir, cnt_dir_entry *entry);
int CNTCloseDir(cnt_dir *dir);

int ARCInitHandle(void *bin, arc_handle_t *handle);

#endif // _RVL_CNT_H
