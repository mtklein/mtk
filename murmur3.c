#include "murmur3.h"
#include <string.h>

__attribute__((no_sanitize("unsigned-integer-overflow")))
uint32_t mix(uint32_t hash) {
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
    return hash;
}

__attribute__((no_sanitize("unsigned-integer-overflow")))
uint32_t murmur3(uint32_t hash, const void* vptr, const size_t len) {
    const char* ptr = vptr;
    size_t bytes = len;

    for (; bytes >= 4; bytes -= 4, ptr += 4) {
        uint32_t v;
        memcpy(&v, ptr, sizeof v);

        v *= 0xcc9e2d51;
        v = (v<<15) | (v>>17);
        v *= 0x1b873593;

        hash ^= v;

        hash = (hash<<13) | (hash>>19);
        hash *= 5;
        hash += 0xe6546b64;
    }

    uint32_t v = 0;
    switch (bytes) {
        case 3: v |= (uint32_t)ptr[2] << 16;
        case 2: v |= (uint32_t)ptr[1] <<  8;
        case 1: v |= (uint32_t)ptr[0] <<  0;
                v *= 0xcc9e2d51;
                v = (v<<15) | (v>>17);
                v *= 0x1b873593;

                hash ^= v;
    }

    hash ^= (uint32_t)len;
    return mix(hash);
}
