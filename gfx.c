#include "gfx.h"
#include <stdint.h>

#if defined(__aarch64__)
    // TODO: is there a Clang idiom to generate ld4.?? instructions from portable code?
    #include <arm_neon.h>

    #define cast __builtin_convertvector

    // I could not coax Clang to use ucvtf.8h for this no matter how I tried,
    // whether __builtin_convertvector() or vcvtq_f16_u16(), always u8 -> u16 -> u32 -> f32 -> f16.
    // Maybe it's because not all u16 are representable as f16?
    // TODO: maybe convert to u16, or a constant, fma?
    static Half f16_from_u8(uint8x8_t u8) {
    #if 0
        return cast(u8, Half);
    #else
        uint16x8_t u16 = vmovl_u8(u8);
        Half f16;
        __asm__("ucvtf.8h %0,%1" : "=w"(f16) : "w"(u16));
        return f16;
    #endif
    }
#endif


Slab shade_color(void* ctx, Float x, Float y) {
    const Color* c = ctx;
    (void)x;
    (void)y;
    return (Slab) {
        (half)c->r,
        (half)c->g,
        (half)c->b,
        (half)c->a,
    };
}

Slab blend_src(Slab src, Slab dst) {
    (void)dst;
    return src;
}

Slab blend_dst(Slab src, Slab dst) {
    (void)src;
    return dst;
}

Slab blend_srcover(Slab src, Slab dst) {
    src.r += dst.r * (1-src.a);
    src.g += dst.g * (1-src.a);
    src.b += dst.b * (1-src.a);
    src.a += dst.a * (1-src.a);
    return src;
}

#if !defined(__aarch64__)
    #define GENERIC_LOAD              \
        Slab s = {0};                 \
        for (int i = 0; i < N; i++) { \
            s.r[i] = (half)p[i].r;    \
            s.g[i] = (half)p[i].g;    \
            s.b[i] = (half)p[i].b;    \
            s.a[i] = (half)p[i].a;    \
        }
#endif

Slab load_rgba_f16(const void *px) {
#if defined(__aarch64__)
    float16x8x4_t v = vld4q_f16(px);
    Slab s = {
        v.val[0],
        v.val[1],
        v.val[2],
        v.val[3],
    };
#else
    const struct { half r,g,b,a; } *p = px;
    GENERIC_LOAD
#endif
    return s;
}

Slab load_rgba_unorm8(const void *px) {
#if defined(__aarch64__)
    uint8x8x4_t v = vld4_u8(px);
    Slab s = {
        f16_from_u8(v.val[0]),
        f16_from_u8(v.val[1]),
        f16_from_u8(v.val[2]),
        f16_from_u8(v.val[3]),
    };
#else
    const struct { uint8_t r,g,b,a; } *p = px;
    GENERIC_LOAD
#endif
    s.r *= (1/255.0f16);
    s.g *= (1/255.0f16);
    s.b *= (1/255.0f16);
    s.a *= (1/255.0f16);
    return s;
}

Slab load_rgba_unorm16(const void *px) {
    // 0xffff (65535) becomes +inf when converted directly to f16, so we go via f32.
#if defined(__aarch64__)
    uint16x8x4_t v = vld4q_u16(px);
    return (Slab) {
        cast(cast(v.val[0], Float) * (1/65535.0f), Half),
        cast(cast(v.val[1], Float) * (1/65535.0f), Half),
        cast(cast(v.val[2], Float) * (1/65535.0f), Half),
        cast(cast(v.val[3], Float) * (1/65535.0f), Half),
    };
#else
    const struct { uint16_t r,g,b,a; } *p = px;
    Slab s = {0};
    for (int i = 0; i < N; i++) {
        s.r[i] = (half)( (float)p[i].r * (1/65535.0f) );
        s.g[i] = (half)( (float)p[i].g * (1/65535.0f) );
        s.b[i] = (half)( (float)p[i].b * (1/65535.0f) );
        s.a[i] = (half)( (float)p[i].a * (1/65535.0f) );
    }
    return s;
#endif
}
