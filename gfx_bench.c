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
    Step step[] = {
        {.effect=load_rgb_unorm8},  {.ctx=dst},
        {.effect=shade_rgba_f32},   {.ctx=rgba},
        {.effect=blend_srcover},
        {.effect=store_rgb_unorm8}, {.ctx=dst},
        {.effect=done},
    };

    double start = now();
    while (k --> 0) {
        drive(step,len(dst)/3);
    }
    return now() - start;
}

static double rgba_unorm8(int k, double *scale, const char* *unit) {
    *scale = 63;
    *unit  = "px";

    uint8_t dst[63*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };
    Step step[] = {
        {.effect=load_rgba_unorm8},  {.ctx=dst},
        {.effect=shade_rgba_f32},    {.ctx=rgba},
        {.effect=blend_srcover},
        {.effect=store_rgba_unorm8}, {.ctx=dst},
        {.effect=done},
    };

    double start = now();
    while (k --> 0) {
        drive(step,len(dst)/4);
    }
    return now() - start;
}

static double rgba_unorm16(int k, double *scale, const char* *unit) {
    *scale = 63;
    *unit  = "px";

    uint16_t dst[63*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };
    Step step[] = {
        {.effect=load_rgba_unorm16},  {.ctx=dst},
        {.effect=shade_rgba_f32},     {.ctx=rgba},
        {.effect=blend_srcover},
        {.effect=store_rgba_unorm16}, {.ctx=dst},
        {.effect=done},
    };

    double start = now();
    while (k --> 0) {
        drive(step,len(dst)/4);
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
