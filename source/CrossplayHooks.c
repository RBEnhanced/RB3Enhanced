#include "rb3e_include.h"
#include "CrossplayHooks.h"
#include <string.h>

BinStream *OnlineIDReadHook(BinStream *thisBinStream, void *onlineID)
{
    BinstreamRead(thisBinStream, (void **)onlineID, 36);
    return thisBinStream;
}

BinStream *OnlineIDWriteHook(BinStream *thisBinStream, void *onlineID)
{
    char empty[36] = {0};
    BinstreamWrite(thisBinStream, (void *)&empty, 36);
    return thisBinStream;
}

// this is mostly for debugging purposes
// might remove for final release
// however, maybe i could use font glyphs for the platform? might be cool
BinStream *WriteUsernameHook(BinStream *thisBinStream, char *username)
{
    char newUsername[64] = {0}; // Buffer for the new username with platform prefix
    const char *platformPrefix;
    size_t remainingSize = 0;
    size_t usedLength = 0;
#ifdef RB3E_WII
    platformPrefix = "[Wii] ";
#else
    platformPrefix = "[360] ";
#endif

    // cant use strncat on wii so this will have to do
    strncpy(newUsername, platformPrefix, sizeof(newUsername) - 1);
    newUsername[sizeof(newUsername) - 1] = '\0';

    usedLength = strlen(newUsername);
    remainingSize = sizeof(newUsername) - usedLength - 1;

    strncpy(newUsername + usedLength, username, remainingSize);
    newUsername[sizeof(newUsername) - 1] = '\0';
    BinstreamWriteLengthString(thisBinStream, newUsername);
}
