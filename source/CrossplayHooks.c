#ifdef RB3E_WII

#include "rb3e_include.h"

// TODO: Rework every bit of this
// they are fast enough and since they only run once when a player connects, they shouldnt cause any issues, but they are still ugly

void GetUserDataWriteHook(BinStream *this, void *data, int size)
{
    int i = 0;
    char hexString[size * 2 + 1]; // +1 for null terminator
    unsigned char *byteData = (unsigned char *)data;
    Modifier *crossplayModifier;

    crossplayModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.useCrossplayNetcode, 0);

    if (crossplayModifier->enabled)
    {

        int strLength = ((char *)data)[7]; // get the username length

        // i dont care for this way of detecting if the packet is already "ps3ified", figure out a better way to detect it
        // TODO: improve this
        if (strLength > 15)
        {
            BinstreamWrite(this, data, size);
            return;
        }

        // this is what will be stored on the "recent players" list on PS3, its the true PSN/RPCN username
        // all nulls causes a syscall for adding recent players to fail, but doesnt impact anything else
        char empty[0x24] = {0x69, 0x68, 0x61, 0x74, 0x65, 0x63, 0x6F, 0x6D, 0x70, 0x76, 0x69, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        BinstreamWrite(this, data, 8);                                         // write first 8 bytes
        BinstreamWrite(this, data + 8, strLength);                             // write the original username
        BinstreamWrite(this, (void *)empty, 36);                               // write 36 empty bytes
        BinstreamWrite(this, data + 8 + strLength + 4, size - 12 - strLength); // write the rest of the data
    }
    else
    {
        BinstreamWrite(this, data, size);
    }
    return;
}

void GetUserDataWriteEndianHook(BinStream *this, int size)
{
    Modifier *crossplayModifier;

    crossplayModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.useCrossplayNetcode, 0);

    if (crossplayModifier->enabled)
    {
        return BinstreamWriteEndian(this, size + 0x20);
    }
    else
    {
        return BinstreamWriteEndian(this, size);
    }
}

void GetUserDataReadHook(BinStream *this, void *data, int size)
{
    int streamCounter = 0;
    int strLength = 0;
    int i = 0;
    char empty[0x24] = {0};
    Modifier *crossplayModifier;

    crossplayModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.useCrossplayNetcode, 0);

    if (crossplayModifier->enabled)
    {

        BinstreamRead(this, data, 8);
        strLength = ((char *)data)[7];
        BinstreamRead(this, (char *)data + 8, strLength);
        BinstreamRead(this, (void *)empty, 0x24);
        memcpy((char *)data + 8 + strLength, "\x00\x00\x00\x00", 4); // use empty pid on wii
        BinstreamRead(this, (char *)data + 8 + strLength + 4, size - 8 - strLength - 0x24);

        unsigned char *byteData = (unsigned char *)data;
    }
    else
    {
        BinstreamRead(this, data, size);
    }
    return;
}

#endif