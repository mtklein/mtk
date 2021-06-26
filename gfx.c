#include "assume.h"
#include "gfx.h"
#include <stdint.h>
#include <string.h>

#define cast    __builtin_convertvector
#define shuffle __builtin_shufflevector

#if N == 8
    static const F32 iota = {0,1,2,3, 4,5,6,7};

    #define LD3_0  0,3,6, 9, 12,15,18,21
    #define LD3_1  1,4,7,10, 13,16,19,22
    #define LD3_2  2,5,8,11, 14,17,20,23

    #define LD4_0  0,4, 8,12, 16,20,24,28
    #define LD4_1  1,5, 9,13, 17,21,25,29
    #define LD4_2  2,6,10,14, 18,22,26,30
    #define LD4_3  3,7,11,15, 19,23,27,31

    #define CONCAT 0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15

    #define ST3    0, 8,16,    1, 9,17,    2,10,18,    3,11,19,    \
                   4,12,20,    5,13,21,    6,14,22,    7,15,23
    #define ST4    0, 8,16,24, 1, 9,17,25, 2,10,18,26, 3,11,19,27, \
                   4,12,20,28, 5,13,21,29, 6,14,22,30, 7,15,23,31
#else
    static const F32 iota = {0,1,2,3};

    #define LD3_0  0,3,6, 9
    #define LD3_1  1,4,7,10
    #define LD3_2  2,5,8,11

    #define LD4_0  0,4, 8,12
    #define LD4_1  1,5, 9,13
    #define LD4_2  2,6,10,14
    #define LD4_3  3,7,11,15

    #define CONCAT 0,1,2,3, 4,5,6,7

    #define ST3    0,4,8,    1,5,9,    2,6,10,    3,7,11
    #define ST4    0,4,8,12, 1,5,9,13, 2,6,10,14, 3,7,11,15
#endif

#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    typedef int16_t mask;
#else
    typedef int mask;
#endif

typedef mask __attribute__((ext_vector_type(N))) Mask;

typedef uint8_t __attribute__((ext_vector_type(  N))) U8;
typedef uint8_t __attribute__((ext_vector_type(3*N))) U8x3;
typedef uint8_t __attribute__((ext_vector_type(4*N))) U8x4;


typedef uint16_t __attribute__((ext_vector_type(  N))) U16;
typedef uint16_t __attribute__((ext_vector_type(4*N))) U16x4;

typedef _Float16 __attribute__((ext_vector_type(  N))) F16;
typedef _Float16 __attribute__((ext_vector_type(4*N))) F16x4;

typedef half  __attribute__((ext_vector_type(4))) Half4;
typedef float __attribute__((ext_vector_type(4))) Float4;

static Half select(Mask cond, Half t, Half e) { return (Half)( ( cond & (Mask)t)
                                                             | (~cond & (Mask)e) ); }
static Half min(Half x, Half y) { return select(y < x, y, x); }
static Half max(Half x, Half y) { return select(x < y, y, x); }
static Half clamp(Half x, Half lo, Half hi) { return max(lo, min(x, hi)); }

// LLVM won't use uvcvf.8h to convert U16 to Half, otherwise we'd just cast(u8, Half).
static Half Half_from_U8(U8 u8) {
#if 1 && defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    U16  u16 = cast(u8, U16);
    Half half;
    __asm__("ucvtf.8h %0,%1" : "=w"(half) : "w"(u16));
    return half;
#else
    return cast(u8, Half);
#endif
}

RGBA matrix_2x3(void* ctx, RGBA src, Cold* cold) {
    const float* m = ctx;
    F32 x = cold->x * m[0] + (cold->y * m[1] + m[2]),
        y = cold->x * m[3] + (cold->y * m[4] + m[5]);
    cold->x = x;
    cold->y = y;
    return src;
}

RGBA matrix_3x3(void* ctx, RGBA src, Cold* cold) {
    const float* m = ctx;
    F32 x = cold->x * m[0] + (cold->y * m[1] + m[2]),
        y = cold->x * m[3] + (cold->y * m[4] + m[5]),
        z = cold->x * m[6] + (cold->y * m[7] + m[8]);
    cold->x = x * (1/z);
    cold->y = y * (1/z);
    return src;
}

RGBA shade_rgba_f32(void* ctx, RGBA src, Cold* cold) {
    (void)cold;
    Float4 rgba_f32;
    memcpy(&rgba_f32, ctx, sizeof rgba_f32);
    Half4 rgba = cast(rgba_f32, Half4);
    src.r = rgba.r;
    src.g = rgba.g;
    src.b = rgba.b;
    src.a = rgba.a;
    return src;
}

RGBA blend_src(void* ctx, RGBA src, Cold* cold) {
    (void)ctx;
    (void)cold;
    return src;
}

RGBA blend_dst(void* ctx, RGBA src, Cold* cold) {
    (void)ctx;
    src = cold->dst;
    return src;
}

RGBA blend_srcover(void* ctx, RGBA src, Cold* cold) {
    (void)ctx;
    src.r += cold->dst.r * (1-src.a);
    src.g += cold->dst.g * (1-src.a);
    src.b += cold->dst.b * (1-src.a);
    src.a += cold->dst.a * (1-src.a);
    return src;
}

RGBA clamp_01(void* ctx, RGBA src, Cold* cold) {
    (void)ctx;
    (void)cold;
    src.r = clamp(src.r, 0, 1);
    src.g = clamp(src.g, 0, 1);
    src.b = clamp(src.b, 0, 1);
    src.a = clamp(src.a, 0, 1);
    return src;
}

RGBA load_rgba_f16(const void* ptr) {
    F16x4 v = *(const F16x4*)ptr;
    return (RGBA) {
        cast(shuffle(v,v, LD4_0), Half),
        cast(shuffle(v,v, LD4_1), Half),
        cast(shuffle(v,v, LD4_2), Half),
        cast(shuffle(v,v, LD4_3), Half),
    };
}

void store_rgba_f16(void* ptr, RGBA src) {
    F16 r = cast(src.r, F16),
        g = cast(src.g, F16),
        b = cast(src.b, F16),
        a = cast(src.a, F16);
    *(F16x4*)ptr = shuffle(shuffle(r, g, CONCAT),
                           shuffle(b, a, CONCAT), ST4);
}

RGBA load_rgb_unorm8(const void* ptr) {
    U8x3 v = *(const U8x3*)ptr;
    return (RGBA) {
        Half_from_U8(shuffle(v,v, LD3_0)) * (half)(1/255.0),
        Half_from_U8(shuffle(v,v, LD3_1)) * (half)(1/255.0),
        Half_from_U8(shuffle(v,v, LD3_2)) * (half)(1/255.0),
        1,
    };
}

void store_rgb_unorm8(void* ptr, RGBA src) {
    U8 r = cast(src.r * (half)255.0 + (half)0.5, U8),
       g = cast(src.g * (half)255.0 + (half)0.5, U8),
       b = cast(src.b * (half)255.0 + (half)0.5, U8);
    *(U8x3*)ptr = shuffle(shuffle(r, g, CONCAT),
                          shuffle(b, b, CONCAT), ST3);
}

RGBA load_rgba_unorm8(const void* ptr) {
    U8x4 v = *(const U8x4*)ptr;
    return (RGBA) {
        Half_from_U8(shuffle(v,v, LD4_0)) * (half)(1/255.0),
        Half_from_U8(shuffle(v,v, LD4_1)) * (half)(1/255.0),
        Half_from_U8(shuffle(v,v, LD4_2)) * (half)(1/255.0),
        Half_from_U8(shuffle(v,v, LD4_3)) * (half)(1/255.0),
    };
}

void store_rgba_unorm8(void* ptr, RGBA src) {
    U8 r = cast(src.r * (half)255.0 + (half)0.5, U8),
       g = cast(src.g * (half)255.0 + (half)0.5, U8),
       b = cast(src.b * (half)255.0 + (half)0.5, U8),
       a = cast(src.a * (half)255.0 + (half)0.5, U8);
    *(U8x4*)ptr = shuffle(shuffle(r, g, CONCAT),
                          shuffle(b, a, CONCAT), ST4);
}

// 0xffff (65535) becomes +inf when converted directly to f16, so this always goes via f32.
RGBA load_rgba_unorm16(const void* ptr) {
    U16x4 v = *(const U16x4*)ptr;
    return (RGBA) {
        cast( cast(shuffle(v,v, LD4_0), F32) * (1/65535.0f), Half ),
        cast( cast(shuffle(v,v, LD4_1), F32) * (1/65535.0f), Half ),
        cast( cast(shuffle(v,v, LD4_2), F32) * (1/65535.0f), Half ),
        cast( cast(shuffle(v,v, LD4_3), F32) * (1/65535.0f), Half ),
    };
}

void store_rgba_unorm16(void* ptr, RGBA src) {
    U16 r = cast( cast(src.r, F32) * 65535.0f + 0.5f, U16 ),
        g = cast( cast(src.g, F32) * 65535.0f + 0.5f, U16 ),
        b = cast( cast(src.b, F32) * 65535.0f + 0.5f, U16 ),
        a = cast( cast(src.a, F32) * 65535.0f + 0.5f, U16 );
    *(U16x4*)ptr = shuffle(shuffle(r, g, CONCAT),
                           shuffle(b, a, CONCAT), ST4);
}

static void drive1(void* dptr,
                   int x, int y,
                   Load* load, Store* store,
                   Effect* effect[], void* ctx[]) {
    RGBA src  = {0};
    Cold cold = {
        .dst = load(dptr),
        .x   = (F32)x + 0.5f + iota,
        .y   = (F32)y + 0.5f,
    };
    while (*effect) {
        src = (*effect++)(*ctx++,src,&cold);
    }
    store(dptr, src);
}

void drive(void* dptr, int n,
           int x, int y,
           Load* load, Store* store, size_t bpp,
           Effect* effect[], void* ctx[]) {
    while (n >= N) {
        drive1(dptr, x,y, load,store, effect,ctx);

        dptr = (char*)dptr + N*bpp;
        x += N;
        n -= N;
    }
    if (n > 0) {
        assume(bpp <= 16);
        char scratch[N*16] = {0};
        memcpy(scratch, dptr, (size_t)n*bpp);
        drive1(scratch, x,y, load,store, effect,ctx);
        memcpy(dptr, scratch, (size_t)n*bpp);
    }
}
