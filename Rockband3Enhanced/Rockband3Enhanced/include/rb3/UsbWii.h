#ifndef _USBWII_H
#define _USBWII_H
#ifdef RB3E_WII

typedef struct _HIDDevice {
    int deviceID;
    unsigned short vendorID;
    unsigned short productID;
    int token;
} HIDDevice;

extern int UsbWiiGetType(HIDDevice *device);

#endif // RB3E_WII
#endif // _USBWII_H