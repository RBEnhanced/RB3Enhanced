#ifndef _RVL_WAD_H
#define _RVL_WAD_H

// .bin backup files on the SD card

// at offset 0x4 in the file, NOT 0x0
#define BACKUP_WAD_MAGIC 0x426B0001

typedef struct _backup_wad_header_t {
    unsigned int header_size;
    unsigned short type;
    unsigned short version;
    unsigned int console_id;
    unsigned int save_file_count;
    unsigned int save_file_data_size;
    unsigned int content_tmd_size;
    unsigned int content_data_size;
    unsigned int backup_area_size;
    unsigned char included_contents[0x40]; //bitfield
    union {
        unsigned long long id;
        struct {
            unsigned int pad;
            unsigned char code[4];
        } name;
    } title_id;
    unsigned char mac_address[0x06];
    unsigned char reserved[0x02];
    unsigned char pad[0x10];
} backup_wad_header_t;

/*
    files are aligned to 0x40 byte boundaries
    +-------------------------------+
    | backup_wad_header_t header    |
    +-------------------------------+
    | tmd_t tmd of content_tmd_size |
    | padding to 0x40 bytes         |
    +-------------------------------+
    | binary of content_data_size   |
    | padding to 0x40 bytes         |
    +-------------------------------+
*/

#endif // _RVL_WAD_H
