#include "bench.h"
#include "gfx.h"
#include "len.h"
#include <stdint.h>

static double rgb_unorm8(int k) {
    uint8_t dst[63*3] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    Effect* effect[] = {
        shade_rgba_f32,
        blend_srcover,
        NULL,
    };
    void* ctx[] = {
        rgba,
        NULL,
    };

    double start = now();
    while (k --> 0) {
        drive(dst,3, load_rgb_unorm8,
              dst,3,store_rgb_unorm8,
              0,0, len(dst)/3,
              effect,ctx);
    }
    return now() - start;
}

static double rgba_unorm8(int k) {
    uint8_t dst[63*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    Effect* effect[] = {
        shade_rgba_f32,
        blend_srcover,
        NULL,
    };
    void* ctx[] = {
        rgba,
        NULL,
    };

    double start = now();
    while (k --> 0) {
        drive(dst,4, load_rgba_unorm8,
              dst,4,store_rgba_unorm8,
              0,0, len(dst)/4,
              effect,ctx);
    }
    return now() - start;
}

static double rgba_unorm16(int k) {
    uint16_t dst[63*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    Effect* effect[] = {
        shade_rgba_f32,
        blend_srcover,
        NULL,
    };
    void* ctx[] = {
        rgba,
        NULL,
    };

    double start = now();
    while (k --> 0) {
        drive(dst,8, load_rgba_unorm16,
              dst,8,store_rgba_unorm16,
              0,0, len(dst)/4,
              effect,ctx);
    }
    return now() - start;
}

int main(int argc, char** argv) {
    bench( rgb_unorm8);
    bench(rgba_unorm8);
    bench(rgba_unorm16);
    return 0;
}
