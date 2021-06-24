#include "gfx.h"
#include <stdint.h>

#define cast    __builtin_convertvector
#define shuffle __builtin_shufflevector

typedef uint8_t __attribute__((ext_vector_type(  N))) U8;
typedef uint8_t __attribute__((ext_vector_type(3*N))) U8x3;
typedef uint8_t __attribute__((ext_vector_type(4*N))) U8x4;

typedef uint16_t __attribute__((ext_vector_type(  N))) U16;
typedef uint16_t __attribute__((ext_vector_type(4*N))) U16x4;

typedef _Float16 __attribute__((ext_vector_type(4*N))) F16x4;


// I could not coax Clang to use ucvtf.8h for this no matter how I tried,
// whether __builtin_convertvector() or vcvtq_f16_u16(), always u8 -> u16 -> u32 -> f32 -> f16.
// Maybe it's because not all u16 are representable as f16?
// TODO: maybe convert to u16, then or in a constant, then fma?
static F16 F16_from_U8(U8 u8) {
#if 1 && defined(__aarch64__)
    U16 u16 = cast(u8, U16);
    F16 f16;
    __asm__("ucvtf.8h %0,%1" : "=w"(f16) : "w"(u16));
    return f16;
#else
    return cast(u8, F16);
#endif
}

static Slab shade_color(void* vctx, Slab src, Slab dst, F32 x, F32 y) {
    struct Shade_Color* ctx = vctx;
    (void)src;
    (void)dst;
    (void)x;
    (void)y;
    return (Slab) {
        (_Float16)ctx->color.r,
        (_Float16)ctx->color.g,
        (_Float16)ctx->color.b,
        (_Float16)ctx->color.a,
    };
}
const struct Shade_Color shade_color_init = {
    shade_color,
    {0.0f, 0.0f, 0.0f, 0.0f},
};


Slab blend_src(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)ctx;
    (void)dst;
    (void)x;
    (void)y;
    return src;
}

Slab blend_dst(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)ctx;
    (void)src;
    (void)x;
    (void)y;
    return dst;
}

Slab blend_srcover(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)ctx;
    (void)x;
    (void)y;
    src.r += dst.r * (1-src.a);
    src.g += dst.g * (1-src.a);
    src.b += dst.b * (1-src.a);
    src.a += dst.a * (1-src.a);
    return src;
}

#define LD3_0  0,3,6, 9, 12,15,18,21
#define LD3_1  1,4,7,10, 13,16,19,22
#define LD3_2  2,5,8,11, 14,17,20,23

#define LD4_0  0,4, 8,12, 16,20,24,28
#define LD4_1  1,5, 9,13, 17,21,25,29
#define LD4_2  2,6,10,14, 18,22,26,30
#define LD4_3  3,7,11,15, 19,23,27,31

#define CONCAT  0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15

#define ST3  0, 8,16,    1, 9,17,    2,10,18,    3,11,19,    \
             4,12,20,    5,13,21,    6,14,22,    7,15,23
#define ST4  0, 8,16,24, 1, 9,17,25, 2,10,18,26, 3,11,19,27, \
             4,12,20,28, 5,13,21,29, 6,14,22,30, 7,15,23,31

Slab load_rgba_f16(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)src;
    (void)dst;
    (void)x;
    (void)y;
    F16x4 v = *(F16x4*)ctx;
    return (Slab) {
        shuffle(v,v, LD4_0),
        shuffle(v,v, LD4_1),
        shuffle(v,v, LD4_2),
        shuffle(v,v, LD4_3),
    };
}

Slab store_rgba_f16(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)dst;
    (void)x;
    (void)y;
    *(F16x4*)ctx = shuffle(shuffle(src.r, src.g, CONCAT),
                           shuffle(src.b, src.a, CONCAT), ST4);
    return src;
}

Slab load_rgb_unorm8(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)src;
    (void)dst;
    (void)x;
    (void)y;
    U8x3 v = *(U8x3*)ctx;
    return (Slab) {
        F16_from_U8(shuffle(v,v, LD3_0)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD3_1)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD3_2)) * (1/255.0f16),
        1.0f16,
    };
}

Slab store_rgb_unorm8(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)dst;
    (void)x;
    (void)y;
    U8 r = cast(src.r * 255.0f16 + 0.5f16, U8),
       g = cast(src.g * 255.0f16 + 0.5f16, U8),
       b = cast(src.b * 255.0f16 + 0.5f16, U8);
    *(U8x3*)ctx = shuffle(shuffle(r, g, CONCAT),
                          shuffle(b, b, CONCAT), ST3);
    return src;
}

Slab load_rgba_unorm8(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)src;
    (void)dst;
    (void)x;
    (void)y;
    U8x4 v = *(U8x4*)ctx;
    return (Slab) {
        F16_from_U8(shuffle(v,v, LD4_0)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD4_1)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD4_2)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD4_3)) * (1/255.0f16),
    };
}

Slab store_rgba_unorm8(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)dst;
    (void)x;
    (void)y;
    U8 r = cast(src.r * 255.0f16 + 0.5f16, U8),
       g = cast(src.g * 255.0f16 + 0.5f16, U8),
       b = cast(src.b * 255.0f16 + 0.5f16, U8),
       a = cast(src.a * 255.0f16 + 0.5f16, U8);
    *(U8x4*)ctx = shuffle(shuffle(r, g, CONCAT),
                          shuffle(b, a, CONCAT), ST4);
    return src;
}

// 0xffff (65535) becomes +inf when converted directly to f16, so this goes via f32.
Slab load_rgba_unorm16(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)src;
    (void)dst;
    (void)x;
    (void)y;
    U16x4 v = *(U16x4*)ctx;
    return (Slab) {
        cast( cast(shuffle(v,v, LD4_0), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, LD4_1), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, LD4_2), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, LD4_3), F32) * (1/65535.0f), F16 ),
    };
}

Slab store_rgba_unorm16(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)dst;
    (void)x;
    (void)y;
    U16 r = cast( cast(src.r, F32) * 65535.0f + 0.5f, U16 ),
        g = cast( cast(src.g, F32) * 65535.0f + 0.5f, U16 ),
        b = cast( cast(src.b, F32) * 65535.0f + 0.5f, U16 ),
        a = cast( cast(src.a, F32) * 65535.0f + 0.5f, U16 );
    *(U16x4*)ctx = shuffle(shuffle(r, g, CONCAT),
                           shuffle(b, a, CONCAT), ST4);
    return src;
}
