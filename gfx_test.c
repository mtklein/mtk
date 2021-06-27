#include "expect.h"
#include "gfx.h"
#include "len.h"
#include <math.h>

static void test_seed_xy() {
    Cold cold = { .X = 2, .Y = 3 };
    seed_xy(NULL, (RGBA){0}, &cold);

    expect_eq(cold.x[0], 2.5f);
    expect_eq(cold.x[1], 3.5f);
    expect_eq(cold.y[0], 3.5f);
    expect_eq(cold.y[1], 3.5f);
}

static void test_matrix_2x3() {
    float m[] = { 1,2,3,
                  4,5,6 };
    Cold cold = { .x = 2, .y = 3 };
    matrix_2x3(m, (RGBA){0}, &cold);

    expect_eq(cold.x[0], 2 +  6 + 3);
    expect_eq(cold.y[0], 8 + 15 + 6);
}

static void test_matrix_3x3() {
    float m[] = { 1,2,3,
                  4,5,6,
                  7,8,9 };
    Cold cold = { .x = 2, .y = 3 };
    matrix_3x3(m, (RGBA){0}, &cold);

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

    src = clamp_01(NULL,src,NULL);

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

static void test_load_rgba_f16() {
    const _Float16 px[4*N] = { 0.0, 0.25, 0.5, 0.75, 1.0 };
    RGBA s = load_rgba_f16(px);

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

static void test_load_rgb_unorm8() {
    const uint8_t px[3*N] = { 0x00, 0x55, 0xaa, 0xfe, 0xff };
    RGBA s = load_rgb_unorm8(px);

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
    const uint8_t px[4*N] = { 0x00, 0x55, 0xaa, 0xfe, 0xff };
    RGBA s = load_rgba_unorm8(px);

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
    const uint16_t px[4*N] = { 0x0000, 0x5555, 0xaaaa, 0xffee, 0xffff };
    RGBA s = load_rgba_unorm16(px);

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
    uint8_t dst[1*4] = {0};

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
    drive(dst,4, load_rgba_unorm8,
          dst,4,store_rgba_unorm8,
          0,0, len(dst)/4,
          effect, ctx);

    for (int i = 0; i < len(dst)/4; i++) {
        expect_eq(dst[4*i+0], 0x55);
        expect_eq(dst[4*i+1], 0x80);
        expect_eq(dst[4*i+2], 0xaa);
        expect_eq(dst[4*i+3], 0xff);
    }
}

static void test_drive_n() {
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
    drive(dst,8, load_rgba_unorm16,
          dst,8,store_rgba_unorm16,
          0,0, len(dst)/4,
          effect,ctx);

    for (int i = 0; i < len(dst)/4; i++) {
        expect_in(dst[4*i+0], 0x553f, 0x5540);
        expect_eq(dst[4*i+1], 0x8000);
        expect_in(dst[4*i+2], 0xaa7e, 0xaa7f);
        expect_eq(dst[4*i+3], 0xffff);
    }
}

int main(void) {
    test_clamp_01();
    test_drive_1();
    test_drive_n();
    test_load_rgb_unorm8();
    test_load_rgba_f16();
    test_load_rgba_unorm16();
    test_load_rgba_unorm8();
    test_matrix_2x3();
    test_matrix_3x3();
    test_seed_xy();
    test_store_rgb_unorm8();
    test_store_rgba_f16();
    test_store_rgba_unorm16();
    test_store_rgba_unorm8();
    return 0;
}
