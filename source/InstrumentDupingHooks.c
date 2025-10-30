

/*
    RB3Enhanced - InstrumentDupingHooks.c
    Hooks dealing with duplicating instruments.
*/

#include "rb3e_include.h"
#include "utilities.h"
#include "InstrumentDupingHooks.h"

static PtrMap* g_DuplicatedGemDBs = NULL;
static PtrMap* g_DuplicatedPhraseDBs = NULL;
static Object* plates[8] = { NULL };
static int numPlates = 0;

void PostLoadHook(GemPlayer* thisGemPlayer, char b) {
    TrackWatcherImpl* watcher;
    GameGemDB* playerGemDB;
    // PhraseDB * phraseDB; TODO: add this

    // call the original PostLoad first
    PostLoad(thisGemPlayer, b);

    watcher = thisGemPlayer->mMatcher->mWatcher->mImpl;

    RB3E_DEBUG("Obtained TrackWatcherImpl %p for GemPlayer %p", watcher, thisGemPlayer);

    playerGemDB = (GameGemDB*)ptr_map_lookup(g_DuplicatedGemDBs, thisGemPlayer);

    RB3E_DEBUG("Looking up duplicated GameGemDB for GemPlayer %p", thisGemPlayer);

    if(!playerGemDB)
    {
        GameGemDB* originalGemDB;
        RB3E_DEBUG("No duplicated GameGemDB found for GemPlayer %p, duplicating now", thisGemPlayer);
        originalGemDB = thisGemPlayer->mMatcher->mSongData->gameGemDbs.data[thisGemPlayer->mTrackNum];
        playerGemDB = Duplicate(originalGemDB);
        ptr_map_insert(g_DuplicatedGemDBs, thisGemPlayer, playerGemDB);
        RB3E_DEBUG("Duplicated GameGemDB for GemPlayer %p at %p", thisGemPlayer, playerGemDB);
    }

    // set the proper gamegemlist for the difficulty they've picked
    watcher->mGameGemList = playerGemDB->mGameGemLists.data[thisGemPlayer->mUser->difficulty];

    // make sure all hit states and flags etc. are reset to a fresh state
    // should handle drop-in/drop-out properly
    Reset(watcher->mGameGemList);
}

void GemPlayerDestructorHook(GemPlayer* thisGemPlayer, int unknown) {
    GameGemDB* playerGemDB;

    RB3E_DEBUG("GemPlayer::Destructor called for GemPlayer %p", thisGemPlayer);

    // call the original destructor first
    GemPlayerDestructor(thisGemPlayer);

    RB3E_DEBUG("GemPlayer::Destructor original function called for GemPlayer %p", thisGemPlayer);

    // look up and free all duplicated GameGemDBs
    playerGemDB = (GameGemDB*)ptr_map_lookup(g_DuplicatedGemDBs, thisGemPlayer);

    if(playerGemDB)
    {
        RB3E_DEBUG("Freeing duplicated GameGemDB %p for GemPlayer %p", playerGemDB, thisGemPlayer);
        ptr_map_remove(g_DuplicatedGemDBs, thisGemPlayer);
    }

    // TODO: still need to do more cleanup, there is a memory leak as-is
}

PhraseList *GetPhraseListHook(int i, int phraseType) {
    RB3E_DEBUG("GetPhraseListHook called with index %d and phraseType %d", i, phraseType);
    return GetPhraseList(i, phraseType);
}

Object* GetFreeSmasherPlateHook(void *thisGemTrackResourceManager, int inst) {
    Object* plate = GetFreeSmasherPlate(thisGemTrackResourceManager, inst);
    Object* newObject = RndDirNewObject();

    // copy the plate RndDir
    ((Object*)((char*)plate + 0x18C))->table->copy(newObject, (Object*)((char*)plate + 0x18C), 0);
    
    return (Object*)((char*)newObject - 0x18C);
}

void ReleaseSmasherPlateHook(void *thisGemTrackResourceMananger, Object* plate) {

    RB3E_DEBUG("ReleaseSmasherPlateHook called for plate %p", plate);
    ReleaseSmasherPlate(thisGemTrackResourceMananger, plate);
}
