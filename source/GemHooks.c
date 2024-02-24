#include "GlobalSymbols.h"
#include "ports.h"
#include "rb3/Mem.h"
#include "rb3/ModifierManager.h"
#include "rb3/GameGem.h"
#include "rb3/Random.h"
#include "rb3/Symbol.h"
#include "rb3/Vector.h"

int WillBeNoStrumHook(GameGemList *thisGameGemList, int *gem)
{
    Modifier *forceHoposModifier;

    forceHoposModifier = ModifierIsActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.forceHopos, 0);
    if (forceHoposModifier->enabled)
    {
        return 1;
    }
    else
    {
        return WillBeNoStrum(thisGameGemList, gem);
    }
}

int *GetWidgetByNameHook(int *gemManager, Symbol sym)
{
    Modifier *colorShuffleModifier;
    Symbol *guitarColors[5] = {&globalSymbols.greenGem, &globalSymbols.redGem, &globalSymbols.yellowGem, &globalSymbols.blueGem, &globalSymbols.orangeGem};
    Symbol *guitarHopoColors[5] = {&globalSymbols.greenHopoGem, &globalSymbols.redHopoGem, &globalSymbols.yellowHopoGem, &globalSymbols.blueHopoGem, &globalSymbols.orangeHopoGem};
    Symbol *drumColors[4] = {&globalSymbols.redDrumGem, &globalSymbols.yellowDrumGem, &globalSymbols.blueDrumGem, &globalSymbols.greenDrumGem};
    Symbol *drumCymbalColors[4] = {&globalSymbols.redCymbalGem, &globalSymbols.yellowCymbalGem, &globalSymbols.blueCymbalGem, &globalSymbols.greenCymbalGem};
    int i = 0;

    colorShuffleModifier = ModifierIsActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.colorShuffle, 0);

    if (colorShuffleModifier->enabled)
    {
        // explicitly check the gem widget name so we dont accidentally try to replace a beat/measure line, kick, piano, or cymbal gem with a standard one as they are all track widgets
        for (i = 0; i < sizeof(guitarColors) / sizeof(Symbol *); i++)
        {
            if (strcmp(sym.sym, guitarColors[i]->sym) == 0)
                return GetWidgetByName(gemManager, *guitarColors[RandomInt(0, 5)]);
        }
        for (i = 0; i < sizeof(guitarHopoColors) / sizeof(Symbol *); i++)
        {
            if (strcmp(sym.sym, guitarHopoColors[i]->sym) == 0)
                return GetWidgetByName(gemManager, *guitarHopoColors[RandomInt(0, 5)]);
        }
        for (i = 0; i < sizeof(drumColors) / sizeof(Symbol *); i++)
        {
            if (strcmp(sym.sym, drumColors[i]->sym) == 0)
                return GetWidgetByName(gemManager, *drumColors[RandomInt(0, 4)]);
        }
        for (i = 0; i < sizeof(drumCymbalColors) / sizeof(Symbol *); i++)
        {
            if (strcmp(sym.sym, drumCymbalColors[i]->sym) == 0)
                return GetWidgetByName(gemManager, *drumCymbalColors[RandomInt(0, 4)]);
        }
        return GetWidgetByName(gemManager, sym);
    }
    else
    {
        return GetWidgetByName(gemManager, sym);
    }
}

Symbol GetSlotColorHook(int *bandUser, int slot)
{
    Modifier *colorShuffleModifier;
    Symbol *colors[5] = {&globalSymbols.green, &globalSymbols.red, &globalSymbols.yellow, &globalSymbols.blue, &globalSymbols.orange};

    Symbol slotColor = GetSlotColor(bandUser, slot);

    colorShuffleModifier = ModifierIsActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.colorShuffle, 0);

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

void shuffleColors(GameGem *gem)
{
    char colors[5] = {gem->green, gem->red, gem->yellow, gem->blue, gem->orange};
    int i;

    for (i = 4; i > 0; i--)
    {
        int j = RandomInt(0, 0x7FFFFFFF) % (i + 1);
        char temp = colors[i];
        colors[i] = colors[j];
        colors[j] = temp;
    }

    // Assign the shuffled colors back
    gem->green = colors[0];
    gem->red = colors[1];
    gem->yellow = colors[2];
    gem->blue = colors[3];
    gem->orange = colors[4];
}

int AddGameGemHook(GameGemList *gameGemList, GameGem *gem, NoStrumState gemType)
{
    Modifier *mirrorModeModifier;
    Modifier *gemShuffleModifier;
    char origGreen = gem->green;
    char origRed = gem->red;
    char origBlue = gem->blue;
    char origOrange = gem->orange;

    mirrorModeModifier = ModifierIsActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.mirrorMode, 0);
    gemShuffleModifier = ModifierIsActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.gemShuffle, 0);

    if (gemShuffleModifier->enabled)
    {
        shuffleColors(gem);
    }
    if (mirrorModeModifier->enabled)
    {
        gem->green = origOrange;
        gem->red = origBlue;
        // yellow doesn't need to be swapped
        gem->blue = origRed;
        gem->orange = origGreen;
        return AddGameGem(gameGemList, gem, gemType);
    }
    else
    {
        return AddGameGem(gameGemList, gem, gemType);
    }
}