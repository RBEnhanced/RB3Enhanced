#ifndef _RVL_CNT_H
#define _RVL_CNT_H

// -- ARC, on-disk --

// https://wiki.tockdom.com/wiki/U8_(File_Format)
// header of ARC/U8 files
typedef struct _arc_header_t {
    unsigned int magic;
    int first_node;
    int nodes_size;
    int data_offset;
    int reserved[4];
} arc_header_t; // 0x20

// the types of nodes within an ARC
typedef enum _arc_node_type_t {
    arcnode_file = 0,
    arcnode_dir = 1
} arc_node_type_t;

// info about a file node
typedef struct _arc_node_file_t {
    int data_offset;
    int data_size;
} arc_node_file_t;
// info about a directory node
typedef struct _arc_node_dir_t {
    int parent_node_index;
    int next_node;
} arc_node_dir_t;

// an ARC node itself
typedef struct _arc_node_t {
    int node_type : 8;
    int string_offset : 24;
    union {
        arc_node_file_t file;
        arc_node_dir_t dir;
    };
} arc_node_t; // 0xC

// -- CNT, internally --

typedef enum _cnt_handle_type {
    cnthandle_nand = 0,
    cnthandle_dvd = 1
} cnt_handle_type;

typedef struct _cnt_handle {
    unsigned char backing[0x28]; // TODO: split between ARC/NAND and DVD
    unsigned char type;
} cnt_handle;

#endif // _RVL_CNT_H
