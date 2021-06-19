#pragma once

#include <stdlib.h>

#define push(ptr, n)                                                                \
    (ptr = (n && (n & (n-1))) ? ptr                                                 \
         :                      realloc(ptr, (size_t)(n ? 2*n : 1) * sizeof *ptr)), \
    ptr[n++]

#define pop(ptr, n)                                                   \
    --n,                                                              \
    (ptr = (n && (n & (n-1))) ? ptr                                   \
         :                  n ? realloc(ptr, (size_t)n * sizeof *ptr) \
         :                      (free(ptr), NULL))
