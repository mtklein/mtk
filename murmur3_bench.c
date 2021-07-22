#include "bench.h"
#include "murmur3.h"

static double bench_mix(int k, double *scale, const char* *unit) {
    uint32_t seed = 0;

    *scale = sizeof seed;
    *unit  = "byte";

    double start = now();
    while (k --> 0) {
        (void)mix(seed);
    }
    return now() - start;
}

static double bench_murmur3(int k, double *scale, const char* *unit) {
    char buf[1024] = {0};

    *scale = sizeof buf;
    *unit  = "byte";

    double start = now();
    while (k --> 0) {
        (void)murmur3(0, buf,sizeof buf);
    }
    return now() - start;
}

int main(int argc, char** argv) {
    bench(bench_mix);
    bench(bench_murmur3);
    return 0;
}

