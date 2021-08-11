#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void pretty_int   (int      x) { dprintf(2, "%d\n"    , x); }
static void pretty_u32   (uint32_t x) { dprintf(2, "0x%08x\n", x); }
static void pretty_double(double   x) { dprintf(2, "%g\n"    , x); }

#define pretty(v) _Generic(v  \
    , int:      pretty_int(v) \
    , uint32_t: pretty_u32(v) \
    , default:  pretty_double((double)(v)))

#define expect_(x,op,y) do {                                                      \
    if (!( (x) op (y) )) {                                                        \
        dprintf(2, "%s:%d expect(%s %s %s) failed\n",                             \
                __FILE__, __LINE__, #x, #op, #y);                                 \
        if (!__builtin_constant_p(x)) { dprintf(2, "\t" #x " was "); pretty(x); } \
        if (!__builtin_constant_p(y)) { dprintf(2, "\t" #y " was "); pretty(y); } \
        abort();                                                                  \
    }                                                                             \
} while(0)

#define expect(cond)       expect_(cond,==,1)
#define expect_eq(x,y)     expect_(x,==,y)
#define expect_ne(x,y)     expect_(x,!=,y)
#define expect_lt(x,y)     expect_(x, <,y)
#define expect_in(x,lo,hi) expect_(lo,<=,x); expect_(x,<=,hi)

static inline double now() {
    struct timespec ts;
    expect_eq(0, clock_gettime(CLOCK_MONOTONIC, &ts));
    return (double)ts.tv_sec + 1e-9 * ts.tv_nsec;
}

static inline void bench_(const char* name, double(*fn)(int, double*, const char**)) {
    double      scale = 1.0;
    const char* unit  = "";

    double limit = 0.125;
    for (const char* BENCH_SEC = getenv("BENCH_SEC"); BENCH_SEC;) {
        limit = atof(BENCH_SEC);
        break;
    }

    int k = 1;
    double elapsed = 0;
    while (elapsed < limit) {
        k *= 2;
        elapsed = fn(k, &scale,&unit);
    }

    double rate = k/elapsed * scale;
    const char* powers[] = { "", "k", "M", "G", "T" };
    const char* *power = powers;
    while (rate > 1000) {
        rate /= 1000;
        power++;
    }

    printf("%-30s\t%.3g\t%s%s\n", name,rate,*power,unit);
}

#define bench(fn) if (argc == 1 || 0 == strcmp(argv[1], #fn)) bench_(#fn,fn)
