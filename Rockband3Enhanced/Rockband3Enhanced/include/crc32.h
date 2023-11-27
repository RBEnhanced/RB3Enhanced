/*
    Header file for public domain CRC32 code, from http://home.thep.lu.se/~bjorn/crc/crc32_simple.c
*/

#include <stddef.h>
#include <stdint.h>

void crc32(const void *data, size_t n_bytes, uint32_t *crc);
