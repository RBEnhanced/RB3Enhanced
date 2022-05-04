/*
    RB3Enhanced - gocentral.h
    GoCentral connection hooks.
*/

// RFC-5737 reserved IPv4 address
#define REDIRECT_IP_ADDRESS "192.0.2.1"
#define REDIRECT_IP_ADDRESS_INT 0xc0000201
#define REDIRECT_IP_ADDRESS_CHARS {192, 0, 2, 1}

void *SetAddressHook(void *thisInetAddress, char *addr);