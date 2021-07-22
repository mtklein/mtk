#include "bench.h"
#include "expect.h"
#include "hash.h"
#include <stdlib.h>

static double bench_insert(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};

    double start = now();
    for (int i = 0; i < k; i++) {
        insert(&h,i,i);
    }
    double elapsed = now() - start;
    expect_eq(h.len, k);
    free(h.table);
    return elapsed;
}

static bool always_match(int val, void* vctx) {
    (void)vctx;
    (void)val;
    return true;
}

static double bench_hit(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};
    for (int i = 0; i < 128; i++) {
        insert(&h,i,i*2);
    }

    double start = now();
    while (k --> 0) {
        int i = 42, val;
        expect(lookup(&h,i, always_match,NULL, &val) && val == 84);
    }
    double elapsed = now() - start;
    free(h.table);
    return elapsed;
}

static double bench_miss(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};
    for (int i = 0; i < 128; i++) {
        insert(&h,i,i*2);
    }

    double start = now();
    while (k --> 0) {
        int i = 420, val;
        expect(!lookup(&h,i, always_match,NULL, &val));
    }
    double elapsed = now() - start;
    free(h.table);
    return elapsed;
}

int main(int argc, char** argv) {
    bench(bench_insert);
    bench(bench_hit);
    bench(bench_miss);
    return 0;
}
