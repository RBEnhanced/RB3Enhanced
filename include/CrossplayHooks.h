#include "rb3/BinStream.h"

extern void MessageBrokerDDL(unsigned int doClassID);
BinStream *OnlineIDReadHook(BinStream *thisBinStream, void *onlineID);
BinStream *OnlineIDWriteHook(BinStream *thisBinStream, void *onlineID);
BinStream *WriteUsernameHook(BinStream *thisBinStream, char *username);