#ifndef _RVL_TMD_H
#define _RVL_TMD_H

typedef struct _tmd_header_t {
    unsigned int signature_type; // assumes RSA-2048 / 0x00010001
    unsigned char signature[0x100];
    unsigned char padding[0x3C];
    unsigned char issuer[0x40];
    unsigned char version;
    unsigned char ca_crl_version;
    unsigned char signer_crl_version;
    unsigned char vwii;
    unsigned long long ios_version;
    union {
        unsigned long long id;
        struct {
            unsigned int pad;
            unsigned char code[4];
        } name;
    } title_id;
    unsigned int title_type;
    unsigned short group_id;
    unsigned short padding2;
    unsigned short region;
    unsigned char ratings[0x10];
    unsigned char padding3[0xC];
    unsigned char ipc_mask[0xC];
    unsigned char padding4[0x12];
    unsigned int access_rights;
    unsigned short title_version;
    unsigned short num_contents;
    unsigned short boot_index;
    unsigned short padding5;
} __attribute__((packed)) tmd_header_t;

typedef struct _tmd_content_t {
    unsigned int id;
    unsigned short index;
    unsigned short type;
    unsigned long long size;
    unsigned char SHA1[0x14];
} __attribute__((packed)) tmd_content_t;

#endif // _RVL_TMD_H
