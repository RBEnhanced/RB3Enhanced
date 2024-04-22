#ifndef _NETMESSENGER_H
#define _NETMESSENGER_H

#include "rb3/BinStream.h"
#include "rb3/TextStream.h"

typedef struct _NetMessage NetMessage;

typedef void (*NetMessageDestructor_t)(NetMessage *thisNetMessage, int unk);
typedef void (*NetMessageSave_t)(NetMessage *thisNetMessage, BinStream *binStream);
typedef void (*NetMessageLoad_t)(NetMessage *thisNetMessage, BinStream *binStream);
typedef void (*NetMessageDispatch_t)(NetMessage *thisNetMessage);
typedef char (*NetMessageVoiceData_t)(NetMessage *thisNetMessage);
typedef void (*NetMessagePrint_t)(NetMessage *thisNetMessage, TextStream *thisTextStream);
typedef unsigned char (*NetMessageByteCode_t)(NetMessage *thisNetMessage);
typedef char *(*NetMessageName_t)(NetMessage *thisNetMessage);

typedef struct _NetMessage_vtable
{
    // wii specific rtti stuff
#ifdef RB3E_WII
    void *rtti;
    int unk;
#endif

    NetMessageDestructor_t destructor; // self-explanatory
    NetMessageSave_t save;             // serializes the net message into a stream
    NetMessageLoad_t load;             // deserializes the net message from a stream
    NetMessageDispatch_t dispatch;     // dispatches the net message (this is ran on recv)
    NetMessageVoiceData_t voiceData;   // ?? not quite sure what this one is
    NetMessagePrint_t print;           // prints the net message into a textstream, if possible
    NetMessageByteCode_t byteCode;     // the "bytecode" of the net message (i.e. its internal numeric ID)
    NetMessageName_t name;             // the name of the net message (DataArrayMsg, etc.)

} NetMessage_vtable;

// used to synchronize arbitrary data
// this is the parent class of all messages that get synchronized P2P through the MessageBroker
typedef struct _NetMessage
{
    NetMessage_vtable *vtable;
};

typedef struct _NetMessenger
{
    unsigned int mLastSender;
} NetMessenger;

extern void NetMessengerDispatchMsg(NetMessenger *thisNetMessenger, int *sender, unsigned char byteCode, BinStream *buffer);
extern NetMessage *NetMessageFactoryCreateNetMessage(void *thisNetMessageFactory, unsigned char byteCode);

#endif // _NETMESSENGER_H