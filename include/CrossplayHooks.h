#include "rb3/BinStream.h"

extern void MessageBrokerDDL(unsigned int doClassID);
void MessageBrokerDDLHook(unsigned int doClassID);
void GetUserDataWriteEndianHook(BinStream *this, void *data, int size);
void GetUserDataWriteHook(BinStream *this, void *data, int size);
void GetUserDataFromWii(BinStream *this, void *data, int size);