

/*
    RB3Enhanced - InstrumentDupingHooks.c
    Hooks dealing with duplicating instruments.
*/

#include "rb3e_include.h"
#include "utilities.h"
#include "InstrumentDupingHooks.h"

static PtrMap* g_DuplicatedGemDBs = NULL;
static PtrMap* g_DuplicatedPhraseDBs = NULL;

void PostLoadHook(GemPlayer* thisGemPlayer, char b) {
    TrackWatcherImpl* watcher;
    GameGemDB* playerGemDB;
    // PhraseDB * phraseDB; TODO: add this

    RB3E_DEBUG("GemPlayer::PostLoadHook called", 0);

    // call the original function first
    PostLoad(thisGemPlayer, b);

    RB3E_DEBUG("GemPlayer::PostLoadHook original function called", 0);

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

    watcher->mGameGemList = playerGemDB->mGameGemLists.data[thisGemPlayer->mUser->difficulty];

    // make sure all hit states and flags etc. are reset to a fresh state
    // should handle drop-in/drop-out properly
    Reset(watcher->mGameGemList);
}

void GemPlayerDestructorHook(GemPlayer* thisGemPlayer, int unknown) {
    GameGemDB* playerGemDB;

    RB3E_DEBUG("GemPlayer::DestructorHook called for GemPlayer %p", thisGemPlayer);

    // call the original destructor first
    GemPlayerDestructor(thisGemPlayer);

    RB3E_DEBUG("GemPlayer::DestructorHook original function called for GemPlayer %p", thisGemPlayer);

    // look up and free duplicated GameGemDB
    playerGemDB = (GameGemDB*)ptr_map_lookup(g_DuplicatedGemDBs, thisGemPlayer);

    if(playerGemDB)
    {
        RB3E_DEBUG("Freeing duplicated GameGemDB %p for GemPlayer %p", playerGemDB, thisGemPlayer);
        ptr_map_remove(g_DuplicatedGemDBs, thisGemPlayer);
    }
}
