#include "gfx.h"
#include <stdint.h>

#define cast    __builtin_convertvector
#define shuffle __builtin_shufflevector

typedef uint8_t __attribute__((ext_vector_type(  N))) U8;
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

#define C0 0,4, 8,12, 16,20,24,28
#define C1 1,5, 9,13, 17,21,25,29
#define C2 2,6,10,14, 18,22,26,30
#define C3 3,7,11,15, 19,23,27,31

#define INTERLACE 0,8, 1,9, 2,10, 3,11, 4,12, 5,13, 6,14, 7,15

Slab load_rgba_f16(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)src;
    (void)dst;
    (void)x;
    (void)y;
    F16x4 v = *(F16x4*)ctx;
    return (Slab) {
        shuffle(v,v, C0),
        shuffle(v,v, C1),
        shuffle(v,v, C2),
        shuffle(v,v, C3),
    };
}

Slab store_rgba_f16(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    // TODO: Clang doesn't quite generate st4.8h here yet, but it's next best:
    // zip1.8h  v4, v0, v1
    // zip1.8h  v5, v2, v3
    // st2.4s   { v4, v5 }, [x0], #32
    // zip2.8h  v4, v0, v1
    // zip2.8h  v5, v2, v3
    // st2.4s   { v4, v5 }, [x0]
    // ret
    (void)dst;
    (void)x;
    (void)y;
    F16x4* p = ctx;
    *p = (F16x4)shuffle((F32)shuffle(src.r, src.g, INTERLACE),
                        (F32)shuffle(src.b, src.a, INTERLACE), INTERLACE);
    return src;
}

Slab load_rgba_unorm8(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)src;
    (void)dst;
    (void)x;
    (void)y;
    U8x4 v = *(U8x4*)ctx;
    return (Slab) {
        F16_from_U8(shuffle(v,v, C0)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, C1)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, C2)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, C3)) * (1/255.0f16),
    };
}

// 0xffff (65535) becomes +inf when converted directly to f16, so this goes via f32.
Slab load_rgba_unorm16(void* ctx, Slab src, Slab dst, F32 x, F32 y) {
    (void)src;
    (void)dst;
    (void)x;
    (void)y;
    U16x4 v = *(U16x4*)ctx;
    return (Slab) {
        cast( cast(shuffle(v,v, C0), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, C1), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, C2), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, C3), F32) * (1/65535.0f), F16 ),
    };
}
