#include "hash.h"
#include "bench.h"
#include "expect.h"
#include <stdlib.h>

static _Bool ptr_eq(const void* a, const void* b) {
    return a == b;
}

static double bench_insert(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = { .eq=ptr_eq };

    double start = now();
    for (int i = 1; i <= k; i++) {
        void* p = (void*)(intptr_t)i;
        insert(&h,i,p,p);
    }
    double elapsed = now() - start;
    expect_eq(h.len, k);
    free(h.table);
    return elapsed;
}

static double bench_update(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = { .eq=ptr_eq };
    insert(&h, 42,&h,&h);
    expect_eq(h.len, 1);

    double start = now();
    while (k --> 0) {
        insert(&h,42,&h,&h);
    }
    double elapsed = now() - start;
    expect_eq(h.len, 1);
    free(h.table);
    return elapsed;
}

int main(int argc, char** argv) {
    bench(bench_insert);
    bench(bench_update);
    return 0;
}
