#include "expect.h"
#include "gfx.h"
#include <math.h>

static const Slab empty = {0};
static const F32  zero  = {0};

static void test_clamp_01() {
    Slab src = {
        {+0.0f16, -0.0f16, +1.0f16, -1.0f16},
        {+0.5f16, -0.5f16, +2.0f16, -2.0f16},
        {+(_Float16)INFINITY, -(_Float16)INFINITY},
        {+(_Float16)NAN,      -(_Float16)NAN},
    };

    src = clamp_01(NULL,src,empty,zero,zero);

    expect(src.r[0] == 0.0f16);
    expect(src.r[1] == 0.0f16);
    expect(src.r[2] == 1.0f16);
    expect(src.r[3] == 0.0f16);

    expect(src.g[0] == 0.5f16);
    expect(src.g[1] == 0.0f16);
    expect(src.g[2] == 1.0f16);
    expect(src.g[3] == 0.0f16);

    expect(src.b[0] == 1.0f16);
    expect(src.b[1] == 0.0f16);

    expect(src.a[0] == 0.0f16);
    expect(src.a[1] == 0.0f16);
}

static void test_load_rgba_f16() {
    const _Float16 px[4*N] = { 0.0f16, 0.25f16, 0.5f16, 0.75f16, 1.0f16 };
    Slab s = load_rgba_f16(px);

    expect(s.r[0] == 0.00f16);
    expect(s.g[0] == 0.25f16);
    expect(s.b[0] == 0.50f16);
    expect(s.a[0] == 0.75f16);
    expect(s.r[1] == 1.00f16);
}

static void test_store_rgba_f16() {
    Slab src = {
        {0.00f16, 1.0f16},
        {0.25f16},
        {0.50f16},
        {0.75f16},
    };

    _Float16 px[4*N] = {0};
    store_rgba_f16(px,src);
    expect(px[0] == 0.00f16);
    expect(px[1] == 0.25f16);
    expect(px[2] == 0.50f16);
    expect(px[3] == 0.75f16);
    expect(px[4] == 1.00f16);
}

static void test_load_rgb_unorm8() {
    const uint8_t px[3*N] = { 0x00, 0x55, 0xaa, 0xfe, 0xff };
    Slab s = load_rgb_unorm8(px);

    expect(s.r[0] == 0.0f16);
    expect(0.333f16 < s.g[0] && s.g[0] < 0.334f16);
    expect(0.666f16 < s.b[0] && s.b[0] < 0.667f16);
    expect(s.a[0] == 1.0f16);
    expect(s.r[1] <  1.0f16);
    expect(s.g[1] == 1.0f16);
}

static void test_store_rgb_unorm8() {
    Slab src = {
        {0.000f16, 1.000f16},
        {0.333f16},
        {0.666f16},
        {0.996f16},
    };

    uint8_t px[3*N] = {0};
    store_rgb_unorm8(px,src);
    expect(px[0] == 0x00);
    expect(px[1] == 0x55);
    expect(px[2] == 0xaa);
    expect(px[3] == 0xff);
}

static void test_load_rgba_unorm8() {
    const uint8_t px[4*N] = { 0x00, 0x55, 0xaa, 0xfe, 0xff };
    Slab s = load_rgba_unorm8(px);

    expect(s.r[0] == 0.0f16);
    expect(0.333f16 < s.g[0] && s.g[0] < 0.334f16);
    expect(0.666f16 < s.b[0] && s.b[0] < 0.667f16);
    expect(s.a[0] <  1.0f16);
    expect(s.r[1] == 1.0f16);
}

static void test_store_rgba_unorm8() {
    Slab src = {
        {0.000f16, 1.000f16},
        {0.333f16},
        {0.666f16},
        {0.996f16},
    };

    uint8_t px[4*N] = {0};
    store_rgba_unorm8(px,src);
    expect(px[0] == 0x00);
    expect(px[1] == 0x55);
    expect(px[2] == 0xaa);
    expect(px[3] == 0xfe);
    expect(px[4] == 0xff);
}

static void test_load_rgba_unorm16() {
    const uint16_t px[4*N] = { 0x0000, 0x5555, 0xaaaa, 0xffee, 0xffff };
    Slab s = load_rgba_unorm16(px);

    expect(s.r[0] == 0.0f16);
    expect(0.333f16 < s.g[0] && s.g[0] < 0.334f16);
    expect(0.666f16 < s.b[0] && s.b[0] < 0.667f16);
    expect(s.a[0] <  1.0f16);
    expect(s.r[1] == 1.0f16);
}

static void test_store_rgba_unorm16() {
    Slab src = {
        {0.000f16, 1.000f16},
        {0.333f16},
        {0.666f16},
        {0.999f16},
    };

    uint16_t px[4*N] = {0};
    store_rgba_unorm16(px,src);
    expect(px[0] == 0x0000);
    expect(px[1] == 0x5540);
    expect(px[2] == 0xaa7f);
    expect(px[3] == 0xffbf);
    expect(px[4] == 0xffff);
}

static void test_drive1() {
    uint8_t dst[4] = {0xff, 0xff, 0xff, 0xff};

    struct Shade_Color shade_color = shade_color_init;
    shade_color.color = (Color){ 0.333f, 0.5f, 0.666f, 1.0f };

    Effect* effect[] = {
        shade_color.effect,
        blend_srcover,
        NULL,
    };
    void* ctx[] = {
        &shade_color,
        NULL,
    };
    drive(dst,1, 0,0, load_rgba_unorm8,store_rgba_unorm8,4, effect,ctx);

    expect(dst[0] == 0x55);
    expect(dst[1] == 0x80);
    expect(dst[2] == 0xaa);
    expect(dst[3] == 0xff);
}

int main(void) {
    test_clamp_01();
    test_drive1();
    test_load_rgb_unorm8();
    test_load_rgba_f16();
    test_load_rgba_unorm16();
    test_load_rgba_unorm8();
    test_store_rgb_unorm8();
    test_store_rgba_f16();
    test_store_rgba_unorm16();
    test_store_rgba_unorm8();
    return 0;
}
