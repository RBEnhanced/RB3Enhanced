/*
    RB3Enhanced - gocentral.h
    GoCentral connection hooks.
*/

#ifndef GOCENTRAL_H_
#define GOCENTRAL_H_

// RFC-5737 reserved IPv4 address
#define REDIRECT_IP_ADDRESS "192.0.2.1"
#define REDIRECT_IP_ADDRESS_INT 0xc0000201
#define REDIRECT_IP_ADDRESS_CHARS \
    {                             \
        192, 0, 2, 1              \
    }

void *SetAddressHook(void *thisInetAddress, char *addr);

#ifdef RB3E_WII
extern int StartDNSLookup(void *wiiNetInit, char *addr);
int StartDNSLookupHook(void *wiiNetInit, char *addr);
#endif

#endif // GOCENTRAL_H_