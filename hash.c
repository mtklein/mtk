#include "hash.h"
#include <stdlib.h>

static _Bool match(const Hash* h, int ix, int hash, const void* key) {
    return h->table[ix].hash == hash
        && (h->table[ix].key == key || (h->eq && h->eq(h->table[ix].key,key)));
}


void* lookup(const Hash* h, int hash, const void* key) {
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

void insert(Hash* h, int hash, const void* key, void* val) {
    {
        int ix = hash & (h->cap-1);
        for (int i = 0; i < h->cap; i++) {
            if (h->table[ix].key == NULL) {
                break;
            }
            if (match(h,ix, hash,key)) {
                h->table[ix].val = val;
                return;
            }
            ix = (ix+1) & (h->cap-1);
        }
    }

    if (h->len == h->cap) {
        Hash grown = {
            .eq  = h->eq,
            .cap = h->cap ? h->cap*2 : 1,
        };
        grown.table = calloc((size_t)grown.cap, sizeof *grown.table);

        for (int ix = 0; ix < h->cap; ix++) {
            if (h->table[ix].key) {
                just_insert(&grown, h->table[ix].hash, h->table[ix].key, h->table[ix].val);
            }
        }

        free(h->table);
        *h = grown;
    }

    just_insert(h, hash, key, val);
}
