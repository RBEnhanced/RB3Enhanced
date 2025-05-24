/*
    RB3Enhanced - GlobalSymbols.h
    Symbols that are constructed only once, directly after plugin initialization.
*/

#include "rb3/Symbol.h"

void InitGlobalSymbols();

typedef struct _GlobalSymbols
{
    // dta functions
    Symbol print_debug;
    Symbol rb3e_change_music_speed;
    Symbol rb3e_change_track_speed;
    Symbol rb3e_get_music_speed;
    Symbol rb3e_get_track_speed;
    Symbol rb3e_set_venue;
    Symbol rb3e_is_emulator;
    Symbol rb3e_relaunch_game;
    Symbol rb3e_get_song_count;
    Symbol rb3e_send_event_string;
    Symbol rb3e_get_song_name;
    Symbol rb3e_get_artist;
    Symbol rb3e_get_album;
    Symbol rb3e_get_origin;
    Symbol rb3e_get_genre;
    Symbol rb3e_search_song_name;

    // modifiers
    Symbol forceHopos;
    Symbol colorShuffle;
    Symbol mirrorMode;
    Symbol blackBackground;
    Symbol gemShuffle;
    Symbol doubleBass;

    // gem widgets
    Symbol greenGem;
    Symbol redGem;
    Symbol yellowGem;
    Symbol blueGem;
    Symbol orangeGem;

    // drum gem widgets
    Symbol redDrumGem;
    Symbol yellowDrumGem;
    Symbol blueDrumGem;
    Symbol greenDrumGem;

    // drum cymbal gem widgets
    Symbol redCymbalGem;
    Symbol yellowCymbalGem;
    Symbol blueCymbalGem;
    Symbol greenCymbalGem;

    // hopo widgets
    Symbol greenHopoGem;
    Symbol redHopoGem;
    Symbol yellowHopoGem;
    Symbol blueHopoGem;
    Symbol orangeHopoGem;

    // slot colors
    Symbol green;
    Symbol red;
    Symbol yellow;
    Symbol blue;
    Symbol orange;

    // overshell instrument entries
    Symbol overshell_guitar;
    Symbol overshell_bass;
    Symbol overshell_keys;
    Symbol overshell_drums;
    Symbol overshell_real_guitar;
    Symbol overshell_real_bass;
    Symbol overshell_real_keys;
    Symbol overshell_drums_pro;
    Symbol overshell_vocal_solo;
    Symbol overshell_vocal_harmony;
} GlobalSymbols;

extern GlobalSymbols globalSymbols;
