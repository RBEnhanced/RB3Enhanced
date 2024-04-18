#include "rb3e_include.h"
#include "CrossplayHooks.h"

void MessageBrokerDDLHook(unsigned int doClassID)
{
    RB3E_DEBUG("Initing MessageBroker DOClass with orig ID %i, overriding to 36", doClassID);
    MessageBrokerDDL(36);
    return;
}

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