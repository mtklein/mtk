#include "array.h"
#include <stdbool.h>
#include <stdlib.h>

__attribute__((no_sanitize("unsigned-integer-overflow")))
static inline bool is_pow2_or_zero(size_t n) {
    return (n & (n-1)) == 0;
}

void* grow(void* ptr, size_t n, size_t sizeofT) {
    if (is_pow2_or_zero(n)) {
        ptr = realloc(ptr, (n ? 2*n : 1) * sizeofT);
    }
    return ptr;
}

void* shrink(void* ptr, size_t n, size_t sizeofT) {
    if (is_pow2_or_zero(n)) {
        if (n) {
            ptr = realloc(ptr, n * sizeofT);
        } else {
            free(ptr);
            ptr = NULL;
        }
    }
    return ptr;
}
