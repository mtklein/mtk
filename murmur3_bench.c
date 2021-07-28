#include "bench.h"
#include "expect.h"
#include "murmur3.h"

static uint32_t seed = 0;

static double bench_mix(int k, double *scale, const char* *unit) {
    *scale = sizeof seed;
    *unit  = "B";

    double start = now();
    while (k --> 0) {
        expect_eq(0, mix(seed));
    }
    return now() - start;
}

static double bench_murmur3(int k, double *scale, const char* *unit) {
    char buf[1024] = {0};
    memcpy(buf, &seed, sizeof seed);

    *scale = sizeof buf;
    *unit  = "B";

    double start = now();
    while (k --> 0) {
        expect_eq(0xa97a91ff, murmur3(0, buf,sizeof buf));
    }
    return now() - start;
}

int main(int argc, char** argv) {
    if (argc > 100) {
        seed = 42;
    }

    bench(bench_mix);
    bench(bench_murmur3);
    return 0;
}

