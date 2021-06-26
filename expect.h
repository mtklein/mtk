#pragma once

#include <stdio.h>
#include <stdlib.h>

#define expect_(x,op,y) do {                                                               \
    if (!( (x) op (y) )) {                                                                 \
        fprintf(stderr, "%s:%d expect(%s %s %s) failed\n",                                 \
                __FILE__, __LINE__, #x, #op, #y);                                          \
        if (!__builtin_constant_p(x)) { fprintf(stderr, "\t%s = %g\n", #x, (double)(x)); } \
        if (!__builtin_constant_p(y)) { fprintf(stderr, "\t%s = %g\n", #y, (double)(y)); } \
        abort();                                                                           \
    }                                                                                      \
} while(0)

#define expect(cond)       expect_(cond,==,1)
#define expect_eq(x,y)     expect_(x,==,y)
#define expect_lt(x,y)     expect_(x, <,y)
#define expect_in(x,lo,hi) expect_(lo,<=,x); expect_(x,<=,hi)
