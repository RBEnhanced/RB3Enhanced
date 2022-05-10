#include <string.h>
#include "rb3/Symbol.h"
#include "GlobalSymbols.h"

GlobalSymbols globalSymbols;

void InitGlobalSymbols()
{
    memset(&globalSymbols, 0, sizeof(globalSymbols));

    SymbolConstruct(&globalSymbols.print_debug, "print_debug");
    SymbolConstruct(&globalSymbols.rb3e_change_music_speed, "rb3e_change_music_speed");
    SymbolConstruct(&globalSymbols.rb3e_change_track_speed, "rb3e_change_track_speed");
    SymbolConstruct(&globalSymbols.rb3e_get_music_speed, "rb3e_get_music_speed");
    SymbolConstruct(&globalSymbols.rb3e_get_track_speed, "rb3e_get_track_speed");
    SymbolConstruct(&globalSymbols.rb3e_set_venue, "rb3e_set_venue");

    SymbolConstruct(&globalSymbols.blackBackground, "mod_black_background");
    SymbolConstruct(&globalSymbols.colorShuffle, "mod_color_shuffle");
    SymbolConstruct(&globalSymbols.forceHopos, "mod_force_hopos");
    SymbolConstruct(&globalSymbols.mirrorMode, "mod_mirror_mode");

    SymbolConstruct(&globalSymbols.greenGem, "gem_green.wid");
    SymbolConstruct(&globalSymbols.redGem, "gem_red.wid");
    SymbolConstruct(&globalSymbols.yellowGem, "gem_yellow.wid");
    SymbolConstruct(&globalSymbols.blueGem, "gem_blue.wid");
    SymbolConstruct(&globalSymbols.orangeGem, "gem_orange.wid");

    SymbolConstruct(&globalSymbols.greenHopoGem, "gem_green_hopo.wid");
    SymbolConstruct(&globalSymbols.redHopoGem, "gem_red_hopo.wid");
    SymbolConstruct(&globalSymbols.yellowHopoGem, "gem_yellow_hopo.wid");
    SymbolConstruct(&globalSymbols.blueHopoGem, "gem_blue_hopo.wid");
    SymbolConstruct(&globalSymbols.orangeHopoGem, "gem_orange_hopo.wid");

    SymbolConstruct(&globalSymbols.green, "green");
    SymbolConstruct(&globalSymbols.red, "red");
    SymbolConstruct(&globalSymbols.yellow, "yellow");
    SymbolConstruct(&globalSymbols.blue, "blue");
    SymbolConstruct(&globalSymbols.orange, "orange");
}
