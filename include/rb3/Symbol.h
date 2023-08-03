#ifndef _SYMBOL_H
#define _SYMBOL_H

typedef struct _Symbol
{
    char *sym;
} Symbol;

// does a lookup in the symbol table, creates a symbol and returns a ptr, or returns a ptr to the existing symbol if one exists
extern Symbol *SymbolConstruct(Symbol *thisSymbol, const char *sym);

extern void SymbolPreInit(int stringTableSize, int hashTableSize);

#endif // _SYMBOL_H
