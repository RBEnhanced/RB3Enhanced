#ifdef RB3E_PS3

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "rb3/File.h"
#include "rb3/Mem.h"
#include "stfs/stfs_structs.h"
#include "rb3enhanced.h"
#include "ports.h"

#define LE16(i) ((((i) & 0xFF) << 8 | ((i) >> 8) & 0xFF) & 0xFFFF)
#define LE(i) (((i) & 0xff) << 24 | ((i) & 0xff00) << 8 | ((i) & 0xff0000) >> 8 | ((i) >> 24) & 0xff)

File_vtable AsyncFileSTFS_vtable;

typedef struct _STFSFileTransaction
{
    void *file;

    int offset_in_file;
    void *target_buffer;
    int bytes_to_read;
    int bytes_read;

    char is_finished;
} STFSFileTransaction;

typedef struct _AsyncFileSTFS
{
    File_vtable *vtable;

    char *stfs_file_path;
    int stfs_file_fd;
    STFSHeader stfs_header;

    char *file_name;
    STFSFileEntry file_entry;

    char do_png_ps3_hack;
    char do_mogg_hack;

    int starting_block;
    int block_count;
    int *block_numbers;
    STFSHashEntry *block_hashes;

    char has_block_cache;
    int cached_block_num;
    uint8_t block_cache[0x1000];

    int cur_pos;

    char is_busy;
    char has_failed;
    STFSFileTransaction active_transaction;
} AsyncFileSTFS;

AsyncFileSTFS *AsyncFileSTFS_Destroy(AsyncFileSTFS *file)
{
    RB3E_DEBUG("AsyncFileSTFS_Destroy(file=%p)", file);
    if (file->stfs_file_fd)
        RB3E_CloseFile(file->stfs_file_fd);
    MemFree(file->block_numbers);
    MemFree(file->block_hashes);
    MemFree(file);
    return NULL; //??? no ???
}

String *AsyncFileSTFS_Filename(String *str, AsyncFileSTFS *file)
{
    RB3E_DEBUG("AsyncFileSTFS_Filename(str=%p, file=%p)", str, file);
    str->length = strlen(file->file_name);
    str->buf = file->file_name;
    return str;
}

int AsyncFileSTFS_Seek(AsyncFileSTFS *file, int iOffset, int iSeekType)
{
    RB3E_DEBUG("AsyncFileSTFS_Seek(file=%p, iOffset=%i, iSeekType=%i)", file, iOffset, iSeekType);
    // can't seek when busy
    if (file->is_busy)
        return -1;

    switch (iSeekType)
    {
    case SEEK_SET:
        file->cur_pos = iOffset;
        break;
    case SEEK_CUR:
        file->cur_pos += iOffset;
        break;
    case SEEK_END:
        file->cur_pos = file->file_entry.file_size - iOffset;
        break;
    }

    return file->cur_pos;
}

int AsyncFileSTFS_Tell(AsyncFileSTFS *file)
{
    RB3E_DEBUG("AsyncFileSTFS_Tell(file=%p)", file);
    return file->cur_pos;
}

void AsyncFileSTFS_Flush(AsyncFileSTFS *file)
{
    RB3E_DEBUG("AsyncFileSTFS_Flush(file=%p)", file);
    return;
}

char AsyncFileSTFS_Eof(AsyncFileSTFS *file)
{
    RB3E_DEBUG("AsyncFileSTFS_Eof(file=%p)", file);
    return (file->cur_pos >= file->file_entry.file_size);
}

char AsyncFileSTFS_Fail(AsyncFileSTFS *file)
{
    RB3E_DEBUG("AsyncFileSTFS_Fail(file=%p)", file);
    return file->has_failed;
}

int AsyncFileSTFS_Size(AsyncFileSTFS *file)
{
    RB3E_DEBUG("AsyncFileSTFS_Size(file=%p)", file);
    return file->file_entry.file_size;
}

char AsyncFileSTFS_ReadDone(AsyncFileSTFS *file, int *oBytes)
{
    RB3E_DEBUG("AsyncFileSTFS_ReadDone(file=%p)", file);
    if (oBytes != NULL)
        *oBytes = file->active_transaction.bytes_read;
    return (file->active_transaction.is_finished == 1);
}

static void ConvertWcharToChar(wchar_t *string, int max, char *out)
{
    for (int i = 0; i < max; i++)
    {
        out[i] = (char)(((uint16_t *)string)[i] & 0xFF);
        if (((uint16_t *)string)[i] == 0x0000)
            break;
    }
}

static int STFS_ReadInt24(uint8_t *int24_bytes, char le)
{
    uint8_t parse_as_uint32[4];
    parse_as_uint32[0] = 0;
    if (!le)
        memcpy(parse_as_uint32 + 1, int24_bytes, 3);
    else
    {
        parse_as_uint32[3] = int24_bytes[0];
        parse_as_uint32[2] = int24_bytes[1];
        parse_as_uint32[1] = int24_bytes[2];
    }
    uint32_t uns32 = *(uint32_t *)parse_as_uint32;
    if (uns32 > 0x7FFFFF)
        uns32 = 0xFFFFFFFF - (uns32 - 0x800000);
    int r = uns32;
    return r;
}

static int STFS_ReadHashEntryForBlock(int file_fd, STFSHeader header, int block_num, STFSHashEntry *out_hash_entry)
{
    int record_in_table = block_num % 0xAA;
    int table_size_shift = header.entry_id >= 0xB000 ? 1 : 0;

    int table_start_block = (block_num / 0xAA) * (table_size_shift == 1 ? 0xAC : 0xAB);
    if (block_num >= 0xAA)
        table_start_block += ((block_num / 0x70E4) + 1) << table_size_shift;
    if (block_num >= 0x70E4)
        table_start_block += 1 << table_size_shift;

    int table_start_addr = 0xB000 + (table_start_block * 0x1000);
    return RB3E_ReadFile(file_fd, table_start_addr + (record_in_table * sizeof(STFSHashEntry)), out_hash_entry, sizeof(STFSHashEntry));
}

static int STFS_ReadDataBlock(int file_fd, STFSHeader header, int block_num, uint8_t *out_block_0x1000)
{
    // skip over the hash tables
    int block_adjust = 0;
    int table_size_shift = header.entry_id >= 0xB000 ? 1 : 0;
    if (block_num >= 0xAA)
        block_adjust += ((block_num / 0xAA)) + 1 << table_size_shift;
    if (block_num > 0x70E4)
        block_adjust += ((block_num / 0x70E4) + 1) << table_size_shift;
    int block_to_read = block_num + block_adjust;
    // get the offset in the file the data block will be read from
    uint32_t file_offset = (block_to_read * 0x1000) + 0xC000;
    return RB3E_ReadFile(file_fd, file_offset, out_block_0x1000, 0x1000);
}

static int STFS_FindFileEntry(int file_fd, STFSHeader header, char *file_path, STFSFileEntry *out_entry)
{
    // chop off any leading slash
    if (file_path[0] == '/')
        file_path++;

    // count the number of subdirectories to seek through
    int num_subdirs = 0;
    for (int i = 0; i < strlen(file_path); i++)
        if (file_path[i] == '/')
            num_subdirs++;

    // read the entire file table
    uint8_t *file_table = MemAlloc(LE16(header.volume_descriptor.block_count) * 0x1000, 0);
    if (file_table == NULL)
    {
        RB3E_MSG("Couldn't allocate RAM for the file table!", NULL);
        return 0;
    }
    int ft_block_num = STFS_ReadInt24(header.volume_descriptor.block_num_int24, 1);
    RB3E_DEBUG("Reading %i file table blocks starting with %i", LE16(header.volume_descriptor.block_count), ft_block_num);
    for (int i = 0; i < LE16(header.volume_descriptor.block_count); i++)
    {
        STFSHashEntry ft_hashentry;
        STFS_ReadHashEntryForBlock(file_fd, header, ft_block_num, &ft_hashentry);
        STFS_ReadDataBlock(file_fd, header, ft_block_num, file_table + (i * 0x1000));
        if (ft_hashentry.block_status != 0x80)
        {
            RB3E_MSG("File table @ block %i not marked as used (%02x)", ft_block_num, ft_hashentry.block_status);
            MemFree(file_table);
            return 0;
        }
        ft_block_num = STFS_ReadInt24(ft_hashentry.next_block_int24, 0);
    }

    // get the individual filename piece
    char filename_to_find[0x28];
    memset(filename_to_find, 0, sizeof(filename_to_find));
    for (int i = 0; i < 0x28; i++)
    {
        if (file_path[i] == 0x00 || file_path[i] == '/')
            break;
        filename_to_find[i] = file_path[i];
    }
    file_path += strlen(filename_to_find) + 1;
    RB3E_DEBUG("Doing root search for \"%s\"", filename_to_find);
    uint16_t parent_dir = 0xFFFF;
    char looking_for_dir = num_subdirs > 0;
    STFSFileEntry *ft = (STFSFileEntry *)file_table;
    char found_file_success = 0;
    // scan until we're at the end
    do
    {
        int dir_total = 0;
#ifdef RB3E_PS3
        // HACK: filename rewrites for PS3 compatibility
        if (!looking_for_dir)
        {
            // milo_ps3 is the same length as mid.edat so we take that optimisation
            // game requests .milo_ps3 -> stfs looks up .milo_xbox
            // game requests .mid.edat -> stfs looks up .mid
            // game requests .png_ps3 -> stfs looks up .png_xbox
            if (strlen(filename_to_find) > sizeof("milo_ps3"))
            {
                char *replace_ext = filename_to_find + strlen(filename_to_find) - sizeof("milo_ps3");
                if (strcmp(replace_ext, ".milo_ps3") == 0)
                    strcpy(replace_ext, ".milo_xbox");
                if (strcmp(replace_ext, ".mid.edat") == 0)
                    strcpy(replace_ext, ".mid");
                if (strcmp(replace_ext + 1, ".png_ps3") == 0)
                    strcpy(replace_ext + 1, ".png_xbox");
            }
        }
#endif
        RB3E_DEBUG("Doing search for \"%s\" in parent %04x", filename_to_find, parent_dir);
        for (int i = 0; i < LE16(header.volume_descriptor.block_count) * 0x40; i++)
        {
            if (ft[i].filename[0] == 0x00)
                continue;
            // RB3E_DEBUG("Entry %i: %s (%02x) (%04x)", i, ft[i].filename, ft[i].filename_len_and_flags, ft[i].path_indicator);
            //  we're looking for a directory and this file is a directory
            if (looking_for_dir && (ft[i].filename_len_and_flags & 0x80) == 0x80)
            {
                if (parent_dir == ft[i].path_indicator && strcmp(ft[i].filename, filename_to_find) == 0)
                {
                    RB3E_DEBUG("Found dir \"%s\" at ordinal %04x", ft[i].filename, dir_total);
                    parent_dir = dir_total;
                    break;
                }
            }
            // we're looking for a file in a directory and this file exists
            if (!looking_for_dir &&
                (ft[i].filename_len_and_flags & 0x80) == 0x00 &&
                parent_dir == ft[i].path_indicator)
            {
                if (strcmp(ft[i].filename, filename_to_find) == 0)
                {
                    RB3E_DEBUG("Found file \"%s\"", ft[i].filename);
                    found_file_success = 1;
                    memcpy(out_entry, &ft[i], sizeof(STFSFileEntry));
                    break;
                }
            }
            if ((ft[i].filename_len_and_flags & 0x80) == 0x80)
                dir_total++;
        }
        if (num_subdirs > 0)
        {
            memset(filename_to_find, 0, sizeof(filename_to_find));
            for (int i = 0; i < 0x28; i++)
            {
                if (file_path[i] == 0x00 || file_path[i] == '/')
                    break;
                filename_to_find[i] = file_path[i];
            }

            file_path += strlen(filename_to_find) + 1;
            num_subdirs--;
            if (num_subdirs == 0)
                looking_for_dir = 0;
            else
                looking_for_dir = 1;
        }
        else
        {
            break;
        }
    } while (num_subdirs >= 0);

    MemFree(file_table);

    return found_file_success;
}

static void AsyncFileSTFS_InternalReadBlock(AsyncFileSTFS *file, int block_num)
{
    if (file->has_block_cache && file->cached_block_num == block_num)
        return;
    // maybe we should check for hash errors here, we have a hash table
    int block_lookup = file->block_numbers[block_num];
    int r = STFS_ReadDataBlock(file->stfs_file_fd, file->stfs_header, block_lookup, file->block_cache);
    if (r >= 0)
    {
        file->has_block_cache = 1;
        file->cached_block_num = block_num;
    }
    else
        file->has_block_cache = 0;
}

int AsyncFileSTFS_Read(AsyncFileSTFS *file, void *iData, int iBytes)
{
    RB3E_DEBUG("AsyncFileSTFS_Read(file=%p, iData=%p, iBytes=%i)", file, iData, iBytes);
    // don't do anything if we're busy or EOF
    if (file->is_busy != 0)
        return 0;

    file->is_busy = 1;

    // figure out how many bytes we're gonna be reading and from where
    int file_offset_start = file->cur_pos;
    int file_offset_end = file->cur_pos + iBytes;
    if (file_offset_end >= file->file_entry.file_size)
        file_offset_end = file->file_entry.file_size;
    int bytes_to_read = file_offset_end - file_offset_start;
    int bytes_read = 0;

    int starting_block = file_offset_start / 0x1000;
    int final_block = file_offset_end / 0x1000;
    int offset_in_block = file_offset_start % 0x1000;

    // cast the output buffer like this so we can increment it forwards
    uint8_t *out_buf = iData;

    for (int i = starting_block; i <= final_block; i++)
    {
        if (bytes_to_read <= 0)
            break;
        // make sure that current block is the one that's cached
        AsyncFileSTFS_InternalReadBlock(file, i);
        // read a max of 0x1000 bytes from the block
        int read_from_block = bytes_to_read < (0x1000 - offset_in_block) ? bytes_to_read : (0x1000 - offset_in_block);
        memcpy(out_buf, file->block_cache + offset_in_block, read_from_block);
        // we do not need this many counters man
        out_buf += read_from_block;
        bytes_read += read_from_block;
        file->cur_pos += read_from_block;
        bytes_to_read -= read_from_block;
        offset_in_block = 0;
    }

    // hack to byteswap the png_xbox upon load so it works on ps3
    // this only works because png_ps3 is read by loading the entire file in a single ReadAsync call
    if (file->do_png_ps3_hack && file_offset_start == 0)
    {
        uint8_t *file_buf = iData;
        file_buf += 0x20; // skip over harmonix image header
        for (int i = 0; i < (iBytes - 0x20); i += 2)
        {
            uint8_t b1 = file_buf[i + 0];
            uint8_t b2 = file_buf[i + 1];
            file_buf[i + 0] = b2;
            file_buf[i + 1] = b1;
        }
    }
    // hack to get C3-encrypted moggs to work
    // this only works because VorbisReader reads the header all in one go
    // thanks scott for the fixed keymask
    if (file->do_mogg_hack && file_offset_start == 0 && iBytes == 60000)
    {
        uint8_t *mogg_header_buf = iData;
        static uint8_t c3_key_mask[0x10] = {0xC3, 0xC3, 0xC3, 0xC3, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
        static uint8_t c3_fix_mask[0x10] = {0xA5, 0xCE, 0xFD, 0x06, 0x11, 0x93, 0x23, 0x21, 0xF8, 0x87, 0x85, 0xEA, 0x95, 0xE4, 0x94, 0xD4};
        if (mogg_header_buf[0] == 0xD) // only C3 used 0xD
        {
            int header_end_offset = LE(*(uint32_t *)(mogg_header_buf + 0x4));
            if (header_end_offset < 60000) // sanity check
            {
                header_end_offset -= 0x28; // "seek" to the PS3 keymask
                if (memcmp(mogg_header_buf + header_end_offset, c3_key_mask, 0x10) == 0)
                    memcpy(mogg_header_buf + header_end_offset, c3_fix_mask, 0x10);
            }
        }
    }

    file->active_transaction.bytes_read = bytes_read;

    file->is_busy = 0;
    return bytes_read;
}

char AsyncFileSTFS_ReadAsync(AsyncFileSTFS *file, void *iData, int iBytes)
{
    // nah
    RB3E_DEBUG("AsyncFileSTFS_ReadAsync(file=%p, iData=%p, iBytes=%i)", file, iData, iBytes);
    AsyncFileSTFS_Read(file, iData, iBytes);
    file->active_transaction.is_finished = 1;
    return 1;
}

AsyncFileSTFS *AsyncFileSTFS_New(char *stfs_file_path, char *file_path)
{
    RB3E_DEBUG("AsyncFileSTFS_New(stfs_file_path=%p, file_path=%p)", stfs_file_path, file_path);
    AsyncFileSTFS *newobj = MemAlloc(sizeof(AsyncFileSTFS), 0);
    if (newobj == NULL)
    {
        RB3E_DEBUG("Not enough memory to load new STFS %s!", stfs_file_path);
        return NULL;
    }
    memset(newobj, 0, sizeof(AsyncFileSTFS));

    newobj->vtable = &AsyncFileSTFS_vtable;

    newobj->stfs_file_fd = RB3E_OpenFile(stfs_file_path, 0);
    if (newobj->stfs_file_fd == -1)
    {
        RB3E_DEBUG("Failed to open STFS %s", stfs_file_path);
        MemFree(newobj);
        return NULL;
    }

    int r = RB3E_ReadFile(newobj->stfs_file_fd, 0, &newobj->stfs_header, sizeof(STFSHeader));
    if (r == -1)
    {
        RB3E_DEBUG("Failed to read STFS header %s", stfs_file_path);
        MemFree(newobj);
        return NULL;
    }

    RB3E_MSG("Mounting STFS %s to get file %s", stfs_file_path, file_path);
    char title[96];
    ConvertWcharToChar(newobj->stfs_header.display_names[0], 96, title);
    RB3E_MSG("STFS Title: %s", title);
    RB3E_MSG("Title ID: %08x - Content Type: %08x", newobj->stfs_header.title_id, newobj->stfs_header.content_type);

    if (newobj->stfs_header.title_id != 0x45410914 && // Rock Band 3
        newobj->stfs_header.title_id != 0x45410869 && // Rock Band 2
        newobj->stfs_header.title_id != 0x45410829)   // Rock Band
    {
        RB3E_MSG("Invalid title ID!", NULL);
        MemFree(newobj);
        return NULL;
    }
    if (newobj->stfs_header.content_type != 1 && // Savegame (Customs)
        newobj->stfs_header.content_type != 2)   // DLC (Real)
    {
        RB3E_MSG("Invalid content type!", NULL);
        MemFree(newobj);
        return NULL;
    }

    // find the file in the archive
    if (STFS_FindFileEntry(newobj->stfs_file_fd, newobj->stfs_header, file_path, &newobj->file_entry) == 0)
    {
        RB3E_MSG("Failed to find subfile in STFS package!", NULL);
        MemFree(newobj);
        return NULL;
    }

#ifdef RB3E_PS3
    char *file_ext = file_path + strlen(file_path) - sizeof("png_ps3");
    if (strcmp(file_ext, ".png_ps3") == 0)
        newobj->do_png_ps3_hack = 1;
    if (strcmp(file_ext + 3, ".mogg") == 0)
        newobj->do_mogg_hack = 1;
#endif

    // prepare allocations for getting the block map of the file
    newobj->block_count = STFS_ReadInt24(newobj->file_entry.blocks_allocated_int24_le, 1);
    newobj->starting_block = STFS_ReadInt24(newobj->file_entry.starting_block_int24_le, 1);
    newobj->block_numbers = MemAlloc(sizeof(int) * newobj->block_count, 0);
    if (newobj->block_numbers == NULL)
    {
        RB3E_MSG("Failed to allocate block number data!", NULL);
        MemFree(newobj);
        return NULL;
    }
    newobj->block_hashes = MemAlloc(sizeof(STFSHashEntry) * newobj->block_count, 0);
    if (newobj->block_hashes == NULL)
    {
        RB3E_MSG("Failed to allocate block hash data!", NULL);
        MemFree(newobj->block_numbers);
        MemFree(newobj);
        return NULL;
    }
    int next_block_num = newobj->starting_block;
    for (int i = 0; i < newobj->block_count; i++)
    {
        newobj->block_numbers[i] = next_block_num;
        STFS_ReadHashEntryForBlock(newobj->stfs_file_fd, newobj->stfs_header, next_block_num, &newobj->block_hashes[i]);
        if (newobj->block_hashes[i].block_status != 0x80)
        {
            RB3E_MSG("File block %i @ block %i not marked as used (%02x)", i, next_block_num, newobj->block_hashes[i].block_status);
            MemFree(newobj->block_numbers);
            MemFree(newobj->block_hashes);
            MemFree(newobj);
            return NULL;
        }
        next_block_num = STFS_ReadInt24(newobj->block_hashes[i].next_block_int24, 0);
    }

    return newobj;
}

void AsyncFileSTFS_InitGlobal()
{
    AsyncFileSTFS_vtable.destructor = &AsyncFileSTFS_Destroy;
    AsyncFileSTFS_vtable.destructor2 = &AsyncFileSTFS_Destroy;
    AsyncFileSTFS_vtable.Filename = &AsyncFileSTFS_Filename;
    AsyncFileSTFS_vtable.Read = &AsyncFileSTFS_Read;
    AsyncFileSTFS_vtable.ReadAsync = &AsyncFileSTFS_ReadAsync;
    // AsyncFileSTFS_vtable.Write = AsyncFileSTFS_Write;
    // AsyncFileSTFS_vtable.WriteAsync = AsyncFileSTFS_WriteAsync;
    AsyncFileSTFS_vtable.Seek = &AsyncFileSTFS_Seek;
    AsyncFileSTFS_vtable.Tell = &AsyncFileSTFS_Tell;
    AsyncFileSTFS_vtable.Flush = &AsyncFileSTFS_Flush;
    AsyncFileSTFS_vtable.Eof = &AsyncFileSTFS_Eof;
    AsyncFileSTFS_vtable.Fail = &AsyncFileSTFS_Fail;
    AsyncFileSTFS_vtable.Size = &AsyncFileSTFS_Size;
    AsyncFileSTFS_vtable.UncompressedSize = &AsyncFileSTFS_Size;
    AsyncFileSTFS_vtable.ReadDone = &AsyncFileSTFS_ReadDone;
    AsyncFileSTFS_vtable.WriteDone = &AsyncFileSTFS_ReadDone;
}

#endif // RB3E_PS3
