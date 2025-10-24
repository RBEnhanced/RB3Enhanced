#ifndef POINTER_MAP_H
#define POINTER_MAP_H

#include <stdlib.h>
#include "uthash.h"

// an entry in the pointer-to-pointer map
typedef struct _PtrMapEntry {
    void *key; // pointer key
    void *value; // pointer value
    UT_hash_handle hh; // internal uthash stuff
} PtrMapEntry;

// a hash-map with pointer keys and pointer values
// functions like a dictionary<void*, void*>
// useful for if you need to map some pointer to some other pointer like we do in instrument duiping
typedef struct _PtrMap {
    PtrMapEntry *head;      // table head ptr
    unsigned int size;      // the amount of entries in the map
} PtrMap;

// creates a new pointer map
// if MemAlloc fails, just returns NULL
// however is MemAlloc is failing you are probably down bad anyway and the game has already crashed
static PtrMap* ptr_map_create()
{
    PtrMap *m = (PtrMap*)MemAlloc(sizeof(PtrMap), 0);
    if (!m) return 0;
    m->head = 0;
    m->size = 0;
    return m;
}

// destroy the pointer map and free all memory
static void ptr_map_destroy(PtrMap* map)
{
    PtrMapEntry *e, *tmp;
    if (!map) return;
    HASH_ITER(hh, map->head, e, tmp) {
        HASH_DEL(map->head, e);
        MemFree(e);
    }
    MemFree(map);
}

// insert or update a key-value pair with a key and value
static void ptr_map_insert(PtrMap* map, void* key, void* value)
{
    PtrMapEntry *e;
    if (!map) return;
    HASH_FIND_PTR(map->head, &key, e);
    if (e) { e->value = value; return; }
    e = (PtrMapEntry*)MemAlloc(sizeof(PtrMapEntry), 0);
    if (!e) return;
    e->key = key;
    e->value = value;
    HASH_ADD_PTR(map->head, key, e);
    map->size++;
}

// lookup a value by key
// return NULL if not found
static void* ptr_map_lookup(PtrMap* map, void* key)
{
    PtrMapEntry *e;
    if (!map) return 0;
    HASH_FIND_PTR(map->head, &key, e);
    return e ? e->value : 0;
}

// remove a key-value pair by key
static void ptr_map_remove(PtrMap* map, void* key)
{
    PtrMapEntry *e;
    if (!map) return;
    HASH_FIND_PTR(map->head, &key, e);
    if (e) {
        HASH_DEL(map->head, e);
        MemFree(e);
        if (map->size) 
        {
            map->size--;
        }
    }
}

#endif // POINTER_MAP_H