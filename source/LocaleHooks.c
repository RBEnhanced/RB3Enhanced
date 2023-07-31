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
#include "rb3enhanced.h"

static char *rb3e_version_motd = "RB3Enhanced " RB3E_BUILDTAG " is active. Thank you for playing!";
static char *rb3e_mod_motd_fmt = "RB3Enhanced " RB3E_BUILDTAG " (with %s) is active. Thank you for playing!";

// Localisation messages to be replaced (that can be replaced in DTA with rb3e_ at the start of the regular name)
// Left: the translation ID as in locale_keep.dta, Right: the text to replace with
static char *overrideLocales[][2] = {
#ifdef RB3E_XBOX // xbox-specific locale changes (re liveless)
    {"waiting_for_remote_players", "Waiting for online players..."},
    {"main_menu_finding", "Finding online players..."},
    {"band_chooser_find", "Find Online Players"},
    {"overshell_online_options", "Online Options"},
    {"overshell_register_online", "Play Online"},
#endif
};
static int numOverrideLocales = sizeof(overrideLocales) / sizeof(overrideLocales[0]);

// New localisation messages to be added (that can be replaced in DTA with the regular name)
// Left: the translation ID as in locale_keep.dta, Right: the text to replace with
static char *newLocales[][2] = {
    {"mod_black_background", "Black Background"},
    {"mod_force_hopos", "Force HOPOs"},
    {"mod_mirror_mode", "Mirror Mode (Guitar Only)"},
    {"mod_color_shuffle", "Gem Color Shuffle"},
};
static int numNewLocales = sizeof(newLocales) / sizeof(newLocales[0]);

static char ActiveLocale[RB3E_LANG_LEN + 1] = {0};

void SetSystemLanguageHook(Symbol lang, int r4)
{
    Symbol language = lang;
    // if the config file has overridden the language, set that
    if (config.LanguageOverride[0] != '\0')
        SymbolConstruct(&language, config.LanguageOverride);
    // also keep track of our language, as we may need to add our own localisations
    strncpy(ActiveLocale, language.sym, RB3E_LANG_LEN);
    RB3E_DEBUG("Setting language '%s'", ActiveLocale);
    SetSystemLanguage(language, r4);
}

int IsSupportedLanguageHook(Symbol lang, int r4)
{
    // le trolleur
    return 1;
}

// this has to be accessible at all times, so can't put it on the stack
static char mod_locale_string[0x80];
char *LocalizeHook(int thisLocale, Symbol sym, int fail)
{
    int i = 0;
    char newLocaleName[0x50];
    Symbol newLocale;
    char *original;
    // game origin icons relies on using different formatting for the song/artist name
    if (config.GameOriginIcons == 1 && sym.sym != NULL && strcmp(sym.sym, "song_artist_fmt") == 0)
        return "%s <it>%s</it>";
    // if the string starts with message_motd_ (but isn't message_motd), it's our motd
    if (memcmp(sym.sym, "message_motd_", 13) == 0)
    {
        // check for "rb3e_mod_string"
        SymbolConstruct(&newLocale, "rb3e_mod_string");
        original = Localize(thisLocale, newLocale, fail);
        if (original == NULL)
            return rb3e_version_motd;
        // make sure the new string won't buffer overflow
        if (strlen(original) >= 0x20)
            return rb3e_version_motd;
        sprintf(mod_locale_string, rb3e_mod_motd_fmt, original);
        return mod_locale_string;
    }
    // if our string is in the override list, use that string entirely
    for (i = 0; i < numOverrideLocales; i++)
    {
        if (strcmp(sym.sym, overrideLocales[i][0]) == 0)
        {
            // check if rb3e_localename exists
            sprintf(newLocaleName, "rb3e_%s", sym.sym);
            SymbolConstruct(&newLocale, newLocaleName);
            original = Localize(thisLocale, newLocale, fail);
            if (original == NULL)
                return overrideLocales[i][1];
            return original;
        }
    }
    // try to localize through to the original string
    original = Localize(thisLocale, sym, fail);
    if (original == NULL)
    {
        // check to see if it's one of our new localisations
        for (i = 0; i < numNewLocales; i++)
            if (strcmp(sym.sym, newLocales[i][0]) == 0)
                return newLocales[i][1];
    }
    return original;
}
