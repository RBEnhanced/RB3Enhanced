#ifndef RB3E_XBOX
#ifndef STFS_STRUCTS_H_
#define STFS_STRUCTS_H_

#include <stdint.h>
#include <wchar.h>

// Referenced from https://www.arkem.org/xbox360-file-reference.pdf

typedef struct _STFSConsoleSignature
{
    uint8_t console_id[5];
    char part_number[0x14];
    uint8_t console_type;
    char mfr_date[0x8];
    uint32_t pub_exponent;
    uint64_t pub_modulus[0x10];
    uint64_t msft_signature[0x20];
    uint64_t console_signature[0x10];
} __attribute__((packed)) STFSConsoleSignature;

typedef struct _STFSLiveSignature
{
    uint64_t msft_signature[0x20];
    uint8_t padding[0x128];
} __attribute__((packed)) STFSLiveSignature;

typedef union _STFSSignature
{
    STFSLiveSignature live;
    STFSConsoleSignature con;
} __attribute__((packed)) STFSSignature;

typedef struct _STFSVolumeDescriptor
{
    uint8_t desc_size;
    uint8_t reserved;
    uint8_t block_seperation;
    uint16_t block_count;
    uint8_t block_num_int24[3];
    uint8_t hash_table_hash[0x14];
    int block_count_alloc;
    int block_count_unalloc;
} __attribute__((packed)) STFSVolumeDescriptor;

typedef struct _STFSLicenseEntry
{
    uint64_t license_data;
    uint32_t bits;
    uint32_t flags;
} __attribute__((packed)) STFSLicenseEntry;

typedef struct _STFSHeader
{
    char magic[4];
    STFSSignature signature;
    STFSLicenseEntry licensing_info[0x10];
    uint8_t header_hash[0x14];
    uint32_t entry_id;
    int content_type;
    int metadata_version;
    int64_t content_size;
    uint32_t media_id;
    int title_version;
    int base_version;
    uint32_t title_id;
    uint8_t platform;
    uint8_t exec_type;
    uint8_t disc_number;
    uint8_t disc_total;
    uint32_t savegame_id;
    uint8_t console_id[5];
    uint8_t profile_id[8];
    STFSVolumeDescriptor volume_descriptor;
    int file_count;
    int64_t combined_file_size;
    uint8_t reserved[0x8];
    uint8_t padding[0x4C];
    uint8_t device_id[0x14];
    wchar_t display_names[0x60][12];
    wchar_t descriptions[0x60][12];
    wchar_t publisher_name[0x40];
    wchar_t title_name[0x40];
    uint8_t transfer_flags;
    int thumb_size;
    int title_thumb_size;
    uint8_t thumbnail[0x4000];
    uint8_t title_thumbnail[0x4000];
} __attribute__((packed)) STFSHeader;

int a = sizeof(STFSHeader);

typedef struct _STFSHashEntry
{
    uint8_t block_hash[0x14];
    uint8_t block_status;
    uint8_t next_block_int24[3];
} __attribute__((packed)) STFSHashEntry;

typedef struct _STFSFileEntry
{
    char filename[0x28];
    uint8_t filename_len_and_flags;
    uint8_t blocks_allocated_int24_le[3];
    uint8_t blocks_allocated2_int24_le[3];
    uint8_t starting_block_int24_le[3];
    uint16_t path_indicator;
    int file_size;
    int last_modified;
    int last_accessed;
} __attribute__((packed)) STFSFileEntry;

int j = sizeof(STFSFileEntry);

#endif // STFS_STRUCTS_H_
#endif // !RB3E_XBOX
