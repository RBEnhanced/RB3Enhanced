/*
    RB3Enhanced - _functions.c
    Definitions used by the compiler to call/hook functions.
    This file should not be included by any other file, to avoid conflicts.
    Functions that are just being called should be unsigned int arrays of size 1, and set up in rb3enhanced.c -> InitialiseFunctions
    Functions that are being hooked should be unsigned int arrays of size 2, and hooked in rb3enhanced.c -> ApplyHooks
*/

#include "ports.h"
#include "ppcasm.h"

// function stub definitions
#ifndef RB3E_WII // AppConstructor is handled by the BrainSlug engine
unsigned int AppConstructor[1] = {1};
#endif
unsigned int ExecuteDTA[1] = {2};
unsigned int SymbolConstruct[1] = {3};
unsigned int ModifierActive[1] = {4};
unsigned int HmxFactoryFuncAt[1] = {5};
unsigned int BandLabelSetDisplayText[1] = {6};
unsigned int RandomInt[1] = {7};
unsigned int DataNodeEvaluate[1] = {8};
// hooked function stubs
unsigned int Localize[2] = {9};
unsigned int SetVenue[2] = {10};
unsigned int ModifierManagerConstructor[2] = {11};
unsigned int NewFile[2] = {12};
unsigned int SongSort[2] = {13};
unsigned int GetSymbolByGameOrigin[2] = {14};
unsigned int WillBeNoStrum[2] = {15};
unsigned int AddGameGem[2] = {16};
unsigned int SetSongAndArtistName[2] = {17};
unsigned int SetMusicSpeed[2] = {18};
unsigned int UpdateTrackSpeed[2] = {19};
unsigned int SetAddress[2] = {20};
unsigned int GetWidgetByName[2] = {21};
unsigned int GetSlotColor[2] = {22};
unsigned int UsbWiiGetType[2] = {23};