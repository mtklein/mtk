#include "array.h"
#include "bench.h"
#include <stddef.h>

static double bench_push(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    float* p = NULL;
    int    n = 0;

    double start = now();
    while (k --> 0) {
        push(p,n) = 1.0f;
    }
    double elapsed = now() - start;
    while (p) {
        pop(p,n);
    }
    return elapsed;
}

static double bench_pop(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    float* p = NULL;
    int    n = 0;

    for (int i = 0; i < k; i++) {
        push(p,n) = 1.0f;
    }

    double start = now();
    while (k --> 0) {
        pop(p,n);
    }
    return now() - start;
}

int main(int argc, char** argv) {
    bench(bench_push);
    bench(bench_pop);
    return 0;
}
