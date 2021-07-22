#pragma once

#include <stdio.h>
#include <stdlib.h>

static void pretty_int   (int      x) { fprintf(stderr, "%d\n"    , x); }
static void pretty_u32   (uint32_t x) { fprintf(stderr, "0x%08x\n", x); }
static void pretty_double(double   x) { fprintf(stderr, "%g\n"    , x); }

#define pretty(v) _Generic(v  \
    , int:      pretty_int(v) \
    , uint32_t: pretty_u32(v) \
    , default:  pretty_double((double)(v)))

#define expect_(x,op,y) do {                                                           \
    if (!( (x) op (y) )) {                                                             \
        fprintf(stderr, "%s:%d expect(%s %s %s) failed\n",                             \
                __FILE__, __LINE__, #x, #op, #y);                                      \
        if (!__builtin_constant_p(x)) { fprintf(stderr, "\t" #x " was "); pretty(x); } \
        if (!__builtin_constant_p(y)) { fprintf(stderr, "\t" #y " was "); pretty(y); } \
        abort();                                                                       \
    }                                                                                  \
} while(0)

#define expect(cond)       expect_(cond,==,1)
#define expect_eq(x,y)     expect_(x,==,y)
#define expect_lt(x,y)     expect_(x, <,y)
#define expect_in(x,lo,hi) expect_(lo,<=,x); expect_(x,<=,hi)
