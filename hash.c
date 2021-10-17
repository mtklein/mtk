#include "assume.h"
#include "hash.h"
#include <stdlib.h>

bool lookup(const Hash h, int hash, bool(*is_this_your_number)(int val, void* ctx), void* ctx) {
    if (hash == 0) { hash = 1; }
    int ix = hash & (h.cap-1);

    for (int i = 0; i < h.cap; i++) {
        if (h.table[ix].hash == hash && is_this_your_number(h.table[ix].val, ctx)) {
            return true;
        }
        if (h.table[ix].hash == 0) {
            return false;
        }
        ix = (ix+1) & (h.cap-1);
    }
    return false;
}

static void just_insert(Hash* h, int hash, int val) {
    assume(h->len < h->cap);

    if (hash == 0) { hash = 1; }
    int ix = hash & (h->cap-1);

    for (int i = 0; i < h->cap; i++) {
        if (h->table[ix].hash == 0) {
            h->table[ix].hash = hash;
            h->table[ix].val  = val;
            h->len++;
            return;
        }
        ix = (ix+1) & (h->cap-1);
    }
    __builtin_unreachable();
}

void insert(Hash* h, int hash, int val) {
    if (h->len >= (h->cap * 3)/4) {
        Hash grown = {
            .cap = h->cap ? h->cap*2 : 1,
        };
        grown.table = calloc((size_t)grown.cap, sizeof *grown.table);

        for (int ix = 0; ix < h->cap; ix++) {
            if (h->table[ix].hash) {
                just_insert(&grown, h->table[ix].hash
                                  , h->table[ix].val);
            }
        }

        free(h->table);
        *h = grown;
    }
    just_insert(h, hash,val);
}
