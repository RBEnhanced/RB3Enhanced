/*
    RB3Enhanced - LocaleHooks.h
    Custom localisation hooks.
*/

#include "rb3/Symbol.h"

char *LocalizeHook(int thisLocale, Symbol sym, int fail);
void SetSystemLanguageHook(Symbol lang, int r4);
int IsSupportedLanguageHook(Symbol lang, int r4);