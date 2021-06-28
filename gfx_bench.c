#include "bench.h"
#include "gfx.h"
#include "len.h"
#include <stdint.h>

static double memset32(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    double start = now();
    uint32_t buf[1024];
    while (k --> 0) {
        uint32_t p = 0xffaaccee;
        memset_pattern4(buf, &p, sizeof buf);
    }
    return now() - start;
}

static double rgb_unorm8(int k, double *scale, const char* *unit) {
    *scale = 63;
    *unit  = "px";

    uint8_t dst[63*3] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    double start = now();
    while (k --> 0) {
        Step step[] = {
            {loadN_rgb_unorm8,  load1_rgb_unorm8,   dst, 3},
            {shade_rgba_f32,    shade_rgba_f32,    rgba, 0},
            {blend_srcover,     blend_srcover,     NULL, 0},
            {storeN_rgb_unorm8, store1_rgb_unorm8,  dst, 3},
            {0},
        };
        drive(step,len(dst)/3,0,0);
    }
    return now() - start;
}

static double rgba_unorm8(int k, double *scale, const char* *unit) {
    *scale = 63;
    *unit  = "px";

    uint8_t dst[63*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    double start = now();
    while (k --> 0) {
        Step step[] = {
            {loadN_rgba_unorm8,  load1_rgba_unorm8,   dst, 4},
            {shade_rgba_f32,     shade_rgba_f32,     rgba, 0},
            {blend_srcover,      blend_srcover,      NULL, 0},
            {storeN_rgba_unorm8, store1_rgba_unorm8,  dst, 4},
            {0},
        };
        drive(step,len(dst)/4,0,0);
    }
    return now() - start;
}

static double rgba_unorm16(int k, double *scale, const char* *unit) {
    *scale = 63;
    *unit  = "px";

    uint16_t dst[63*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    double start = now();
    while (k --> 0) {
        Step step[] = {
            {loadN_rgba_unorm16,  load1_rgba_unorm16,   dst, 8},
            {shade_rgba_f32,      shade_rgba_f32,      rgba, 0},
            {blend_srcover,       blend_srcover,       NULL, 0},
            {storeN_rgba_unorm16, store1_rgba_unorm16,  dst, 8},
            {0},
        };
        drive(step,len(dst)/4,0,0);
    }
    return now() - start;
}

int main(int argc, char** argv) {
    bench(   memset32);
    bench( rgb_unorm8);
    bench(rgba_unorm8);
    bench(rgba_unorm16);
    return 0;
}
