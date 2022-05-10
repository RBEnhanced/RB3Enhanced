#include "GlobalSymbols.h"
#include "ports.h"
#include "rb3/ModifierManager.h"
#include "rb3/GameGem.h"
#include "rb3/Random.h"
#include "rb3/Symbol.h"

int WillBeNoStrumHook(int *gameGemListPtr, int *multiGemInfoPtr)
{
    Modifier *forceHoposModifier;

    forceHoposModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.forceHopos, 0);
    if (forceHoposModifier->enabled)
    {
        return 1;
    }
    else
    {
        return WillBeNoStrum(gameGemListPtr, multiGemInfoPtr);
    }
}

int *GetWidgetByNameHook(int *gemManager, Symbol sym)
{
    Modifier *colorShuffleModifier;
    Symbol *colors[5] = {&globalSymbols.greenGem, &globalSymbols.redGem, &globalSymbols.yellowGem, &globalSymbols.blueGem, &globalSymbols.orangeGem};
    Symbol *colorsHopo[5] = {&globalSymbols.greenHopoGem, &globalSymbols.redHopoGem, &globalSymbols.yellowHopoGem, &globalSymbols.blueHopoGem, &globalSymbols.orangeHopoGem};
    int i = 0;

    colorShuffleModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.colorShuffle, 0);

    if (colorShuffleModifier->enabled)
    {
        // explicitly check the gem widget name so we dont accidentally try to replace a beat/measure line, kick, piano, or cymbal gem with a standard one as they are all track widgets
        for (i = 0; i < sizeof(colors) / sizeof(Symbol *); i++)
        {
            if (strcmp(sym.sym, colors[i]->sym) == 0)
                return GetWidgetByName(gemManager, *colors[RandomInt(0, 5)]);
        }
        for (i = 0; i < sizeof(colorsHopo) / sizeof(Symbol *); i++)
        {
            if (strcmp(sym.sym, colorsHopo[i]->sym) == 0)
                return GetWidgetByName(gemManager, *colorsHopo[RandomInt(0, 5)]);
        }
        return GetWidgetByName(gemManager, sym);
    }
    else
    {
        return GetWidgetByName(gemManager, sym);
    }
}

Symbol GetSlotColorHook(int *bandUser)
{
    Modifier *colorShuffleModifier;
    Symbol *colors[5] = {&globalSymbols.green, &globalSymbols.red, &globalSymbols.yellow, &globalSymbols.blue, &globalSymbols.orange};

    Symbol slotColor = GetSlotColor(bandUser);

    colorShuffleModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.colorShuffle, 0);

    if (colorShuffleModifier->enabled)
    {
        // explicitly check the slot color so we dont accidentally try to make a pro keys gem have a guitar tail (which will make it invisible)
        if (strcmp(slotColor.sym, "green") == 0)
        {
            return *colors[RandomInt(0, 5)];
        }
        if (strcmp(slotColor.sym, "red") == 0)
        {
            return *colors[RandomInt(0, 5)];
        }
        if (strcmp(slotColor.sym, "yellow") == 0)
        {
            return *colors[RandomInt(0, 5)];
        }
        if (strcmp(slotColor.sym, "blue") == 0)
        {
            return *colors[RandomInt(0, 5)];
        }
        if (strcmp(slotColor.sym, "orange") == 0)
        {
            return *colors[RandomInt(0, 5)];
        }
    }

    return slotColor;
}

int AddGameGemHook(int *gameGemList, GameGem *gem, GameGemType gemType)
{
    Modifier *mirrorModeModifier;
    char origGreen = gem->green;
    char origRed = gem->red;
    // char origYellow = gem->yellow;
    char origBlue = gem->blue;
    char origOrange = gem->orange;

    mirrorModeModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.mirrorMode, 0);

    if (mirrorModeModifier->enabled)
    {
        if (gemType == guitar_bass)
        {
            gem->green = origOrange;
            gem->red = origBlue;
            // yellow doesn't need to be swapped
            gem->blue = origRed;
            gem->orange = origGreen;
        }
        return AddGameGem(gameGemList, gem, gemType);
    }
    else
    {
        return AddGameGem(gameGemList, gem, gemType);
    }
}