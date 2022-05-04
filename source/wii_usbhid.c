/*
    RB3Enhanced - wii_usbhid.c
    Functions that handle USB HID related hooks, e.g. PS3 controllers
*/

#ifdef RB3E_WII

#include <stddef.h>
#include <stdio.h>
#include "rb3/UsbWii.h"
#include "ports.h"

int UsbWiiGetTypeHook(HIDDevice *device)
{
    int r = 0;
    if (device->vendorID == 0x12BA)
    { // sony computer entertainment
        switch (device->productID)
        {
        case 0x0100: // gh guitar
        case 0x0200: // rb guitar
            r = 1;
            break;
        case 0x0120: // gh drums
        case 0x0210: // rb drums
            r = 3;
            break;
        }
    }
    else
    {
        r = UsbWiiGetType(device);
    }
    RB3E_DEBUG("Device connected: 0x%04x 0x%04x - type %i", device->vendorID, device->productID, r);
    return r;
}

#endif // RB3E_WII