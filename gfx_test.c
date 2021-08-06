#include "gfx.h"
#include "len.h"
#include "test.h"
#include <math.h>
#include <stdint.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wfloat-equal"

ABI static RGBA trace(Step step[], size_t p, RGBA src, Cold* cold) {
    fprintf(stderr, "p=%zu\nsrc.r={ ", p);
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)src.r[i]); }
    fprintf(stderr, "}\nsrc.g={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)src.g[i]); }
    fprintf(stderr, "}\nsrc.b={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)src.b[i]); }
    fprintf(stderr, "}\nsrc.a={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)src.a[i]); }
    fprintf(stderr, "}\ndst.r={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)cold->dst.r[i]); }
    fprintf(stderr, "}\ndst.g={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)cold->dst.g[i]); }
    fprintf(stderr, "}\ndst.b={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)cold->dst.b[i]); }
    fprintf(stderr, "}\ndst.a={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)cold->dst.a[i]); }
    fprintf(stderr, "}\nx={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)cold->x[i]); }
    fprintf(stderr, "}\ny={ ");
    for (int i = 0; i < N; i++) { fprintf(stderr, "%g ", (double)cold->y[i]); }
    fprintf(stderr, "}\n");

    return step->effect(step+1,p,src,cold);
}

static const RGBA zero;

static void test_seed_xy() {
    int xy[] = {2,7};
    Cold cold = {0};
    Step step[] = {{.ptr=xy}, {.effect=done}};
    seed_xy(step,8,zero,&cold);

    expect_eq(cold.x[0], 10.5f);
    expect_eq(cold.x[1], 11.5f);
    expect_eq(cold.y[0],  7.5f);
    expect_eq(cold.y[1],  7.5f);
}

static void test_matrix_2x3() {
    float m[] = { 1,2,3,
                  4,5,6 };
    Cold cold = { .x = 2, .y = 3 };
    Step step[] = {{.ptr=m}, {.effect=done}};
    matrix_2x3(step,0,zero,&cold);

    expect_eq(cold.x[0], 2 +  6 + 3);
    expect_eq(cold.y[0], 8 + 15 + 6);
}

static void test_matrix_3x3() {
    float m[] = { 1,2,3,
                  4,5,6,
                  7,8,9 };
    Cold cold = { .x = 2, .y = 3 };
    Step step[] = {{.ptr=m}, {.effect=done}};
    matrix_3x3(step,0,zero,&cold);

    expect_eq(cold.x[0], (2 +  6 + 3) * (1.0f/(14 + 24 + 9)));
    expect_eq(cold.y[0], (8 + 15 + 6) * (1.0f/(14 + 24 + 9)));
}

static void test_clamp_01() {
    RGBA src = {
        {+0.0, -0.0, +1.0, -1.0},
        {+0.5, -0.5, +2.0, -2.0},
        {+INFINITY, -INFINITY},
        {+NAN,      -NAN},
    };

    Step step[] = {{.effect=done}};
    src = clamp_01(step,0,src,NULL);

    expect_eq(src.r[0], (half)0.0);
    expect_eq(src.r[1], (half)0.0);
    expect_eq(src.r[2], (half)1.0);
    expect_eq(src.r[3], (half)0.0);

    expect_eq(src.g[0], (half)0.5);
    expect_eq(src.g[1], (half)0.0);
    expect_eq(src.g[2], (half)1.0);
    expect_eq(src.g[3], (half)0.0);

    expect_eq(src.b[0], (half)1.0);
    expect_eq(src.b[1], (half)0.0);

    expect_eq(src.a[0], (half)0.0);
    expect_eq(src.a[1], (half)0.0);
}

#if defined(__FLT16_MIN__)
    static void test_load_rgba_f16() {
        _Float16 px[4*N] = { 0.0, 0.25, 0.5, 0.75, 1.0 };
        RGBA s = load_rgba_f16(px,zero);

        expect_eq(s.r[0], (half)0.00);
        expect_eq(s.g[0], (half)0.25);
        expect_eq(s.b[0], (half)0.50);
        expect_eq(s.a[0], (half)0.75);
        expect_eq(s.r[1], (half)1.00);
    }

    static void test_store_rgba_f16() {
        RGBA src = {
            {0.00, 1.0},
            {0.25},
            {0.50},
            {0.75},
        };

        _Float16 px[4*N] = {0};
        store_rgba_f16(px,src);
        expect_eq(px[0], 0.00f16);
        expect_eq(px[1], 0.25f16);
        expect_eq(px[2], 0.50f16);
        expect_eq(px[3], 0.75f16);
        expect_eq(px[4], 1.00f16);
    }
#endif

static void test_load_rgb_unorm8() {
    uint8_t px[3*N] = { 0x00, 0x55, 0xaa, 0xfe, 0xff };
    RGBA s = load_rgb_unorm8(px,zero);

    expect_eq(s.r[0], (half)0.0);
    expect_in(s.g[0], (half)0.333, (half)0.334);
    expect_in(s.b[0], (half)0.666, (half)0.667);
    expect_eq(s.a[0], (half)1.0);
    expect_lt(s.r[1], (half)1.0);
    expect_eq(s.g[1], (half)1.0);
}

static void test_store_rgb_unorm8() {
    RGBA src = {
        {(half)0.000, (half)1.000},
        {(half)0.333},
        {(half)0.666},
        {(half)0.996},
    };

    uint8_t px[3*N] = {0};
    store_rgb_unorm8(px,src);
    expect_eq(px[0], 0x00);
    expect_eq(px[1], 0x55);
    expect_eq(px[2], 0xaa);
    expect_eq(px[3], 0xff);
}

static void test_load_rgba_unorm8() {
    uint8_t px[4*N] = { 0x00, 0x55, 0xaa, 0xfe, 0xff };
    RGBA s = load_rgba_unorm8(px,zero);

    expect_eq(s.r[0], (half)0.0);
    expect_in(s.g[0], (half)0.333, (half)0.334);
    expect_in(s.b[0], (half)0.666, (half)0.667);
    expect_lt(s.a[0], (half)1.0);
    expect_eq(s.r[1], (half)1.0);
}

static void test_store_rgba_unorm8() {
    RGBA src = {
        {(half)0.000, (half)1.000},
        {(half)0.333},
        {(half)0.666},
        {(half)0.996},
    };

    uint8_t px[4*N] = {0};
    store_rgba_unorm8(px,src);
    expect_eq(px[0], 0x00);
    expect_eq(px[1], 0x55);
    expect_eq(px[2], 0xaa);
    expect_eq(px[3], 0xfe);
    expect_eq(px[4], 0xff);
}

static void test_load_rgba_unorm16() {
    uint16_t px[4*N] = { 0x0000, 0x5555, 0xaaaa, 0xffee, 0xffff };
    RGBA s = load_rgba_unorm16(px,zero);

    expect_eq(s.r[0], (half)0.0);
    expect_in(s.g[0], (half)0.333, (half)0.334);
    expect_in(s.b[0], (half)0.666, (half)0.667);
    expect_lt(s.a[0], (half)1.0);
    expect_eq(s.r[1], (half)1.0);
}

static void test_store_rgba_unorm16() {
    RGBA src = {
        {(half)0.000, (half)1.000},
        {(half)0.333},
        {(half)0.666},
        {(half)0.999},
    };

    uint16_t px[4*N] = {0};
    store_rgba_unorm16(px,src);
    expect_eq(px[0], 0x0000);
    expect_in(px[1], 0x553f, 0x5540);
    expect_in(px[2], 0xaa7e, 0xaa7f);
    expect_in(px[3], 0xffbd, 0xffbf);
    expect_eq(px[4], 0xffff);
}

static void test_drive_1() {
    uint8_t dst[4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };


    Step step[] = {
        {.effect=load}, {.memfn=load_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=done},
    };
    drive(step,len(dst)/4);

    for (int i = 0; i < len(dst)/4; i++) {
        expect_eq(dst[4*i+0], 0x55);
        expect_eq(dst[4*i+1], 0x80);
        expect_eq(dst[4*i+2], 0xaa);
        expect_eq(dst[4*i+3], 0xff);
    }
}

static void test_drive_N() {
    uint8_t dst[N*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    Step step[] = {
        {.effect=load}, {.memfn=load_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=done},
    };
    drive(step,len(dst)/4);

    for (int i = 0; i < len(dst)/4; i++) {
        expect_eq(dst[4*i+0], 0x55);
        expect_eq(dst[4*i+1], 0x80);
        expect_eq(dst[4*i+2], 0xaa);
        expect_eq(dst[4*i+3], 0xff);
    }
}

static void test_drive_Np1() {
    uint8_t dst[(N+1)*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    Step step[] = {
        {.effect=load}, {.memfn=load_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=done},
    };
    drive(step,len(dst)/4);

    for (int i = 0; i < len(dst)/4; i++) {
        expect_eq(dst[4*i+0], 0x55);
        expect_eq(dst[4*i+1], 0x80);
        expect_eq(dst[4*i+2], 0xaa);
        expect_eq(dst[4*i+3], 0xff);
    }
}

static void test_drive_rgb_unorm8() {
    uint8_t dst[63*3] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    Step step[] = {
        {.effect=load}, {.memfn=load_rgb_unorm8}, {.bpp=3}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgb_unorm8}, {.bpp=3}, {.ptr=dst},
        {.effect=done},
    };
    drive(step,len(dst)/3);

    for (int i = 0; i < len(dst)/3; i++) {
        expect_eq(dst[3*i+0], 0x55);
        expect_eq(dst[3*i+1], 0x80);
        expect_eq(dst[3*i+2], 0xaa);
    }
}

static void test_drive_rgba_unorm8() {
    uint8_t dst[63*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    Step step[] = {
        {.effect=load}, {.memfn=load_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=done},
    };
    drive(step,len(dst)/4);

    for (int i = 0; i < len(dst)/4; i++) {
        expect_eq(dst[4*i+0], 0x55);
        expect_eq(dst[4*i+1], 0x80);
        expect_eq(dst[4*i+2], 0xaa);
        expect_eq(dst[4*i+3], 0xff);
    }
}

static void test_drive_rgba_unorm16() {
    uint16_t dst[63*4] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };

    Step step[] = {
        {.effect=load}, {.memfn=load_rgba_unorm16}, {.bpp=8}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgba_unorm16}, {.bpp=8}, {.ptr=dst},
        {.effect=done},
    };
    drive(step,len(dst)/4);

    for (int i = 0; i < len(dst)/4; i++) {
        expect_in(dst[4*i+0], 0x553f, 0x5540);
        expect_eq(dst[4*i+1], 0x8000);
        expect_in(dst[4*i+2], 0xaa7e, 0xaa7f);
        expect_eq(dst[4*i+3], 0xffff);
    }
}


static double rgb_unorm8(int k, double *scale, const char* *unit) {
    *scale = 63;
    *unit  = "px";

    uint8_t dst[63*3] = {0};

    float rgba[] = { 0.333f, 0.5f, 0.666f, 1.0f };
    Step step[] = {
        {.effect=load}, {.memfn=load_rgb_unorm8}, {.bpp=3}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgb_unorm8}, {.bpp=3}, {.ptr=dst},
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
        {.effect=load}, {.memfn=load_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgba_unorm8}, {.bpp=4}, {.ptr=dst},
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
        {.effect=load}, {.memfn=load_rgba_unorm16}, {.bpp=8}, {.ptr=dst},
        {.effect=shade_rgba_f32}, {.ptr=rgba},
        {.effect=blend_srcover},
        {.effect=store}, {.memfn=store_rgba_unorm16}, {.bpp=8}, {.ptr=dst},
        {.effect=done},
    };

    double start = now();
    while (k --> 0) {
        drive(step,len(dst)/4);
    }
    return now() - start;
}

int main(int argc, char** argv) {
    (void)trace;

    test_clamp_01();
    test_drive_1();
    test_drive_N();
    test_drive_Np1();
    test_drive_rgb_unorm8();
    test_drive_rgba_unorm8();
    test_drive_rgba_unorm16();
    test_load_rgb_unorm8();
    test_load_rgba_unorm16();
    test_load_rgba_unorm8();
    test_matrix_2x3();
    test_matrix_3x3();
    test_seed_xy();
    test_store_rgb_unorm8();
    test_store_rgba_unorm16();
    test_store_rgba_unorm8();

#if defined(__FLT16_MIN__)
    test_load_rgba_f16();
    test_store_rgba_f16();
#endif  // TODO: re-enable with portable implementations

    bench( rgb_unorm8);
    bench(rgba_unorm8);
    bench(rgba_unorm16);
    return 0;
}
