#pragma once

#include <stddef.h>

void* grow  (void* ptr, size_t n, size_t sizeofT);
void* shrink(void* ptr, size_t n, size_t sizeofT);

#define push(ptr, n) (ptr = grow(ptr, (size_t)n, sizeof *ptr)), ptr[n++]
#define pop(ptr, n)  --n, (ptr = shrink(ptr, (size_t)n, sizeof *ptr))
