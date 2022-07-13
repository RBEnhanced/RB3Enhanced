#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#ifdef RB3E_XBOX
#include <xtl.h>
#endif

typedef struct _InetAddress
{
    short unknown; // family?
    short port;
#ifdef RB3E_XBOX // TODO: figure out what this is for wii
    IN_ADDR address;
#endif
} InetAddress;

extern void *SetAddress(void *thisInetAddress, char *addr);

#endif // _INETADDRESS_H