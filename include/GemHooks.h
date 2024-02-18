/*
    RB3Enhanced - GemHooks.h
    Hooks for dealing with gems.
*/

#include "rb3/GameGem.h"
#include "rb3/Symbol.h"

int WillBeNoStrumHook(int *gameGemListPtr, int *multiGemInfoPtr);
int *GetWidgetByNameHook(int *gemManager, Symbol name);
int AddGameGemHook(GameGemList *gameGemList, GameGem *gem, NoStrumState noStrumState);
Symbol GetSlotColorHook(int *bandUser);