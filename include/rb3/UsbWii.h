#ifndef _USBWII_H
#define _USBWII_H
#ifdef RB3E_WII

typedef struct _HIDDevice {
    int fd;
    unsigned short vid;
    unsigned short pid;
    unsigned char iInterface;
    unsigned char endpoint_address_in;
    unsigned char endpoint_address_out;
    unsigned char inst;
    void *p_hdd;
    void *p_hcd;
    void *p_hid;
    void *p_hed0;
    void *p_hed1;
} HIDDevice;

typedef struct _UsbDevice {
    HIDDevice *dd; // 0x0
    unsigned int type; // 0x4
    unsigned int state; // 0x8
    unsigned int flags; // 0xC
    unsigned int inactivity; // 0x10
    unsigned char unk_0x14[0x2]; // 0x14
    short ledNum; // 0x16
    unsigned int unk6; // 0x18
    unsigned int buttonMask; // 0x1C
    unsigned short lstickX; // 0x20
    unsigned short lstickY; // 0x22
    unsigned short rstickX; // 0x24
    unsigned short rstickY; // 0x26
    unsigned char pressures[4]; // 0x28
    unsigned char extended[0x10]; // 0x2C
    unsigned char unk_0x3C[0x4]; // 0x3C
    unsigned char packet[0x40]; // 0x40
    unsigned char unk_0x80[0x80]; // 0x80
} UsbDevice; // 0x100

typedef enum _UsbType {
    kUsbNone = 0,
    kUsbGuitar = 1,
    kUsbGuitarRb2 = 2,
    kUsbDrums = 3,
    kUsbDrumsRb2 = 4,
    kUsbMidiGuitarMustang = 5,
    kUsbMidiGuitarSquire = 6,
    kUsbMidiKeyboard = 7,
    kUsbUnused = 8,
    kUsbMidiKeyboardMpa = 9,
    kUsbMidiDrums = 10,
    kUsbTypeMax = 11,
} UsbType;

extern int UsbWiiGetType(HIDDevice *device);

#endif // RB3E_WII
#endif // _USBWII_H