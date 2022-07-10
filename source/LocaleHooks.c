/*
    RB3Enhanced - LocaleHooks.c
    Custom localisation hooks.
*/

#include <string.h>
#include <stdio.h>
#include "version.h"
#include "utilities.h"
#include "ports.h"
#include "config.h"
#include "rb3/Symbol.h"
#include "rb3/Locale.h"

// Full localisations to be replaced.
// Left: the translation ID as in locale_keep.dta, Right: the text to replace with
char *replacementLocales[][2] = {
    {"message_motd_noconnection", "RB3Enhanced " RB3E_BUILDTAG " is active. Thank you for playing!"},
    {"message_motd_signin", "RB3Enhanced " RB3E_BUILDTAG " is active. Thank you for playing!"},
    {"mod_black_background", "Black Background"},
    {"mod_force_hopos", "Force HOPOs"},
    {"mod_mirror_mode", "Mirror Mode (Guitar Only)"},
    {"mod_color_shuffle", "Gem Color Shuffle"},
};
int numReplacementLocales = sizeof(replacementLocales) / sizeof(replacementLocales[0]);

/* Currently broken.
// Substrings in existing localisations to be replaced.
// Left: the string to replace, Right: a string to replace it with
// !!The replacement must be the same length of or shorter than the original!!
char *replacementSubStrings[][2] = {
    {"Xbox LIVE", "GoCentral"},
    {"Rock Central", "GoCentral"},
    {"PlayStation®Network", "GoCentral"},
    {"Nintendo Wi-Fi Connection", "GoCentral"},
};
int numReplacementSubStrings = sizeof(replacementSubStrings) / sizeof(replacementSubStrings[0]);
*/

char *LocalizeHook(int thisLocale, Symbol sym, int fail)
{
    int i = 0;
    char *original;
    // if our string is in the replacement list, use that string entirely
    for (i = 0; i < numReplacementLocales; i++)
    {
        if (strcmp(sym.sym, replacementLocales[i][0]) == 0)
            return replacementLocales[i][1];
    }
    if (config.GameOriginIcons == 1 && sym.sym != NULL && strcmp(sym.sym, "song_artist_fmt") == 0)
        return "%s <it>%s</it>";
    original = Localize(thisLocale, sym, fail);
    if (original != NULL)
    {
        // todo: replacement strings
    }
    else
    {
        // log the failure so we know for later
        RB3E_DEBUG("Could not localize %s", sym.sym);
    }
    return original;
}