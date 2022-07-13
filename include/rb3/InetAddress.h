#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include <xtl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _InetAddress
{
    short unknown; // family?
    short port;
    IN_ADDR address;
} InetAddress;

extern void *SetAddress(void *thisInetAddress, char *addr);

#endif // _INETADDRESS_H