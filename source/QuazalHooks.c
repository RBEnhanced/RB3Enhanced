/*
    RB3Enhanced - QuazalHooks.c
    Hooks for Quazal-related functions.
    Quazal is the creator of the networking middleware used by RB3.
*/

#include "rb3e_include.h"
#include "rb3/BinStream.h"
#include <string.h>

// dirty hack to get the game to print Quazal debug logging out to DbgPrint/OSReport instead
// limited to debug builds as this may introduce crashing or other issues. use with caution
void OperatorEqualsFmtHook(char *r3, char *r4)
{
    RB3E_DEBUG("Quazal: %s", r4);
    return;
}

int StepSequenceJobSetStepHook(int *unk, StepSequenceJobStep *step)
{
    // steps can have no name it seems; make sure we are not trying to print a null pointer
    if (step != NULL && step->jobName != NULL)
    {
        RB3E_DEBUG("Quazal Job: %s", step->jobName);
    }
    return StepSequenceJobSetStep(unk, step);
}

// function is very simple so lets just completely rewrite it and also introduce additional safety checks
void NetMessengerDispatchMsgHook(NetMessenger *thisNetMessenger, int *sender, unsigned char byteCode, BinStream *buffer)
{
    NetMessage *retMsg = {0};

    if (thisNetMessenger == NULL)
    {
        RB3E_DEBUG("NetMessenger is NULL, can't dispatch message", NULL);
        return;
    }

    if (sender == NULL)
    {
        RB3E_DEBUG("Sender is NULL, can't dispatch message", NULL);
        return;
    }

    thisNetMessenger->mLastSender = *sender;

    retMsg = NetMessageFactoryCreateNetMessage((void *)PORT_THENETMESSAGEFACTORY, byteCode);
    if (retMsg == NULL)
    {
        RB3E_DEBUG("Failed to create NetMessage for byteCode %i", byteCode);
        return;
    }

    RB3E_DEBUG("Dispatching NetMessage of class name %s", retMsg->vtable->name(retMsg));
    retMsg->vtable->load(retMsg, buffer);
    retMsg->vtable->dispatch(retMsg);
    retMsg->vtable->destructor(retMsg, 1);

    return;
}