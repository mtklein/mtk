#include "assume.h"
#include "hash.h"
#include <stdbool.h>
#include <stdlib.h>

static bool match(const Hash* h, int ix, int hash, const void* key) {
    if (h->table[ix].hash != hash) { return false; }
    if (h->table[ix].key  == key ) { return  true; }
    return h->eq && h->eq(h->table[ix].key,key, h->ctx);
}

void* lookup(const Hash* h, int hash, const void* key) {
    assume(key);

    int ix = hash & (h->cap-1);
    for (int i = 0; i < h->cap; i++) {
        if (h->table[ix].key == NULL) {
            return NULL;
        }
        if (match(h,ix, hash,key)) {
            return h->table[ix].val;
        }
        ix = (ix+1) & (h->cap-1);
    }
    return NULL;
}

static void just_insert(Hash* h, int hash, const void* key, void* val) {
    assume(key);
    assume(val);
    assume(h->len < h->cap);

    int ix = hash & (h->cap-1);
    for (int i = 0; i < h->cap; i++) {
        if (h->table[ix].key == NULL) {
            h->table[ix].hash = hash;
            h->table[ix].key  = key;
            h->table[ix].val  = val;
            h->len++;
            return;
        }
        ix = (ix+1) & (h->cap-1);
    }
    __builtin_unreachable();
}

static bool maybe_update(Hash* h, int hash, const void* key, void* val) {
    assume(key);
    assume(val);

    int ix = hash & (h->cap-1);
    for (int i = 0; i < h->cap; i++) {
        if (h->table[ix].key == NULL) {
            break;
        }
        if (match(h,ix, hash,key)) {
            h->table[ix].val = val;
            return true;
        }
        ix = (ix+1) & (h->cap-1);
    }
    return false;
}

static void maybe_grow(Hash* h) {
    if (h->len >= (h->cap * 3)/4) {
        Hash grown = {
            .eq  = h->eq,
            .ctx = h->ctx,
            .cap = h->cap ? h->cap*2 : 1,
        };
        grown.table = calloc((size_t)grown.cap, sizeof *grown.table);

        for (int ix = 0; ix < h->cap; ix++) {
            if (h->table[ix].key) {
                just_insert(&grown, h->table[ix].hash
                                  , h->table[ix].key
                                  , h->table[ix].val);
            }
        }

        free(h->table);
        *h = grown;
    }
}

void insert(Hash* h, int hash, const void* key, void* val) {
    if (!maybe_update(h, hash,key,val)) {
        maybe_grow(h);
        just_insert(h, hash,key,val);
    }
}
