#ifndef _LOCALE_H
#define _LOCALE_H

// I believe "fail" being set to 1 means that if there is no locale, it will return the raw Symbol and not a blank string or vice versa
extern char *Localize(int thisLocale, Symbol sym, int fail);
extern void SetSystemLanguage(Symbol lang, int cheat);

#endif // _LOCALE_H