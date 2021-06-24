#include "gfx.h"
#include "expect.h"

static const Slab empty = {0};
static const F32  zero  = {0};

static void test_load_rgba_f16() {
    _Float16 px[4*N] = { 0.0f16, 0.25f16, 0.5f16, 0.75f16, 1.0f16 };
    Slab s = load_rgba_f16(px, empty,empty, zero,zero);

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
    store_rgba_f16(px, src,empty,zero,zero);
    expect(px[0] == 0.00f16);
    expect(px[1] == 0.25f16);
    expect(px[2] == 0.50f16);
    expect(px[3] == 0.75f16);
    expect(px[4] == 1.00f16);
}

static void test_load_rgba_unorm8() {
    uint8_t px[4*N] = { 0x00, 0x55, 0xaa, 0xfe, 0xff };
    Slab s = load_rgba_unorm8(px, empty,empty, zero,zero);

    expect(s.r[0] == 0.0f16);
    expect(0.333f16 < s.g[0] && s.g[0] < 0.334f16);
    expect(0.666f16 < s.b[0] && s.b[0] < 0.667f16);
    expect(s.a[0] <  1.0f16);
    expect(s.r[1] == 1.0f16);
}

static void test_load_rgba_unorm16() {
    uint16_t px[4*N] = { 0x0000, 0x5555, 0xaaaa, 0xffee, 0xffff };
    Slab s = load_rgba_unorm16(px, empty,empty, zero,zero);

    expect(s.r[0] == 0.0f16);
    expect(0.333f16 < s.g[0] && s.g[0] < 0.334f16);
    expect(0.666f16 < s.b[0] && s.b[0] < 0.667f16);
    expect(s.a[0] <  1.0f16);
    expect(s.r[1] == 1.0f16);
}

int main(void) {
    test_load_rgba_f16();
    test_store_rgba_f16();
    test_load_rgba_unorm8();
    test_load_rgba_unorm16();
    return 0;
}
