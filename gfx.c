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

Slab shade_rgba_f32(void* ctx, F32 x, F32 y) {
    struct { float r,g,b,a; } *c = ctx;
    (void)x;
    (void)y;
    return (Slab) {
        (_Float16)c->r,
        (_Float16)c->g,
        (_Float16)c->b,
        (_Float16)c->a,
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

#define C0 0,4, 8,12, 16,20,24,28
#define C1 1,5, 9,13, 17,21,25,29
#define C2 2,6,10,14, 18,22,26,30
#define C3 3,7,11,15, 19,23,27,31

Slab load_rgba_f16(const void *px) {
    F16x4 v = *(const F16x4*)px;
    return (Slab) {
        shuffle(v,v, C0),
        shuffle(v,v, C1),
        shuffle(v,v, C2),
        shuffle(v,v, C3),
    };
}

Slab load_rgba_unorm8(const void *px) {
    U8x4 v = *(const U8x4*)px;
    return (Slab) {
        F16_from_U8(shuffle(v,v, C0)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, C1)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, C2)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, C3)) * (1/255.0f16),
    };
}

// 0xffff (65535) becomes +inf when converted directly to f16, so this goes via f32.
Slab load_rgba_unorm16(const void *px) {
    U16x4 v = *(const U16x4*)px;
    return (Slab) {
        cast( cast(shuffle(v,v, C0), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, C1), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, C2), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, C3), F32) * (1/65535.0f), F16 ),
    };
}
