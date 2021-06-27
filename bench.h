#pragma once

#include <stdio.h>
#include <string.h>
#include <time.h>

static double now() {
    return clock() * (1.0 / CLOCKS_PER_SEC);
}

static void print_rate(double rate) {
    const char* units[] = { "", "k", "M", "G", "T" };
    const char* *unit = units;
    while (rate > 1000) {
        rate /= 1000;
        unit++;
    }
    printf("%.3g%s", rate, *unit);
}

static void bench_(const char* name, double(*fn)(int, double*, const char**)) {
    double      scale = 1.0;
    const char* unit  = "";

    int k = 1;
    double elapsed = 0;
    while (elapsed < 0.125) {
        k *= 2;
        elapsed = fn(k, &scale,&unit);
    }

    printf("%-30s", name);
    print_rate(k/elapsed * scale);
    printf("%s\n", unit);
}

#define bench(fn) if (argc == 1 || 0 == strcmp(argv[1], #fn)) bench_(#fn,fn)
