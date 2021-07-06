#include "hash.h"
#include "bench.h"
#include "expect.h"
#include <stdlib.h>

static double bench_insert(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};

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

    Hash h = {0};
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

static double bench_hit(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};
    for (int i = 1; i <= 128; i++) {
        void* p = (void*)(intptr_t)i;
        insert(&h,i,p,p);
    }

    double start = now();
    while (k --> 0) {
        int i = 42;
        void* p = (void*)(intptr_t)i;
        expect(p == lookup(&h,i,p));
    }
    double elapsed = now() - start;
    free(h.table);
    return elapsed;
}

static double bench_miss(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};
    for (int i = 1; i <= 128; i++) {
        void* p = (void*)(intptr_t)i;
        insert(&h,i,p,p);
    }

    double start = now();
    while (k --> 0) {
        int i = 420;
        void* p = (void*)(intptr_t)i;
        expect(NULL == lookup(&h,i,p));
    }
    double elapsed = now() - start;
    free(h.table);
    return elapsed;
}

int main(int argc, char** argv) {
    bench(bench_insert);
    bench(bench_update);
    bench(bench_hit);
    bench(bench_miss);
    return 0;
}
