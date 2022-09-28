#ifndef _INETADDRESS_H
#define _INETADDRESS_H

typedef struct _InetAddress
{
    short unknown; // family?
    short port;
    unsigned int address;
} InetAddress;

extern void *SetAddress(void *thisInetAddress, char *addr);

#endif // _INETADDRESS_H
