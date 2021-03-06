#include "gfx.h"
#include <stdint.h>
#include <string.h>

#define cast      __builtin_convertvector
#define shuffle   __builtin_shufflevector

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
    typedef int16_t __attribute__((ext_vector_type(N))) Cond;
#else
    typedef int     __attribute__((ext_vector_type(N))) Cond;
#endif

typedef uint8_t  __attribute__((ext_vector_type(N  )            )) U8;
typedef uint8_t  __attribute__((ext_vector_type(N*3), aligned(1))) U8x3;
typedef uint8_t  __attribute__((ext_vector_type(N*4), aligned(1))) U8x4;
typedef uint16_t __attribute__((ext_vector_type(N  )            )) U16;
typedef uint16_t __attribute__((ext_vector_type(N*4), aligned(2))) U16x4;
typedef __fp16   __attribute__((ext_vector_type(N  )            )) F16;
typedef __fp16   __attribute__((ext_vector_type(N*4), aligned(2))) F16x4;

static Half select(Cond cond, Half t, Half f) { return (Half)( ( cond & (Cond)t)
                                                             | (~cond & (Cond)f)); }
static Half min(Half x, Half y) { return select(y < x, y, x); }
static Half max(Half x, Half y) { return select(x < y, y, x); }
static Half clamp(Half x, Half lo, Half hi) { return max(lo, min(x, hi)); }

// LLVM won't use uvcvf.8h to convert U16 to Half, otherwise we'd just cast(u8, Half).
static Half Half_from_U8(U8 u8) {
#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    U16  u16 = cast(u8, U16);
    Half h;
    __asm__("ucvtf.8h %0,%1" : "=w"(h) : "w"(u16));
    return h;
#else
    return cast(u8, Half);
#endif
}

RGBA done(Step step[], size_t p, RGBA src, Cold* cold) {
    (void)step;
    (void)p;
    (void)cold;
    return src;
}

RGBA seed_xy(Step step[], size_t p, RGBA src, Cold* cold) {
    // In any given call to drive(), x marches along one at a time, while y remains constant.
    const int* xy = (step++)->ptr;
    cold->x = (float)(xy[0] + (int)(p/N*N)) + iota + 0.5f;
    cold->y = (float)(xy[1]               )        + 0.5f;
    return step->effect(step+1,p,src,cold);
}

RGBA matrix_2x3(Step step[], size_t p, RGBA src, Cold* cold) {
    const float* m = (step++)->ptr;
    F32 x = cold->x * m[0] + (cold->y * m[1] + m[2]),
        y = cold->x * m[3] + (cold->y * m[4] + m[5]);
    cold->x = x;
    cold->y = y;
    return step->effect(step+1,p,src,cold);
}

RGBA matrix_3x3(Step step[], size_t p, RGBA src, Cold* cold) {
    const float* m = (step++)->ptr;
    F32 x = cold->x * m[0] + (cold->y * m[1] + m[2]),
        y = cold->x * m[3] + (cold->y * m[4] + m[5]),
        z = cold->x * m[6] + (cold->y * m[7] + m[8]);
    cold->x = x * (1/z);
    cold->y = y * (1/z);
    return step->effect(step+1,p,src,cold);
}

RGBA shade_rgba_f32(Step step[], size_t p, RGBA src, Cold* cold) {
    typedef float __attribute__((ext_vector_type(4))) F4;
    typedef half  __attribute__((ext_vector_type(4))) H4;

    F4 rgba_f32;
    memcpy(&rgba_f32, (step++)->ptr, sizeof rgba_f32);

    H4 rgba = cast(rgba_f32, H4);
    src.r = rgba.r;
    src.g = rgba.g;
    src.b = rgba.b;
    src.a = rgba.a;
    return step->effect(step+1,p,src,cold);
}

RGBA blend_src(Step step[], size_t p, RGBA src, Cold* cold) {
    return step->effect(step+1,p,src,cold);
}

RGBA blend_dst(Step step[], size_t p, RGBA src, Cold* cold) {
    src = cold->dst;
    return step->effect(step+1,p,src,cold);
}

RGBA blend_srcover(Step step[], size_t p, RGBA src, Cold* cold) {
    src.r += cold->dst.r * (1-src.a);
    src.g += cold->dst.g * (1-src.a);
    src.b += cold->dst.b * (1-src.a);
    src.a += cold->dst.a * (1-src.a);
    return step->effect(step+1,p,src,cold);
}

RGBA clamp_01(Step step[], size_t p, RGBA src, Cold* cold) {
    src.r = clamp(src.r, 0,1);
    src.g = clamp(src.g, 0,1);
    src.b = clamp(src.b, 0,1);
    src.a = clamp(src.a, 0,1);
    return step->effect(step+1,p,src,cold);
}

RGBA load(Step step[], size_t p, RGBA src, Cold* cold) {
    Memory* fn  =        (step++)->memfn;
    size_t  bpp =        (step++)->bpp;
    void*   ptr = (char*)(step++)->ptr + p/N*N * bpp;

    if (p%N) {
        memcpy(cold->scratch, ptr, bpp*(p%N));
        cold->dst = fn(cold->scratch, src);
        return step->effect(step+1,p,src,cold);
    }
    cold->dst = fn(ptr, src);
    return step->effect(step+1,p,src,cold);
}

RGBA store(Step step[], size_t p, RGBA src, Cold* cold) {
    Memory* fn  =        (step++)->memfn;
    size_t  bpp =        (step++)->bpp;
    void*   ptr = (char*)(step++)->ptr + p/N*N * bpp;

    if (p%N) {
        src = fn(cold->scratch, src);
        memcpy(ptr, cold->scratch, bpp*(p%N));
        return step->effect(step+1,p,src,cold);
    }
    src = fn(ptr, src);
    return step->effect(step+1,p,src,cold);
}

RGBA load_rgba_f16(void* ptr, RGBA src) {
    F16x4 v;
    memcpy(&v, ptr, sizeof v);
    src.r = cast(shuffle(v,v, LD4_0), Half);
    src.g = cast(shuffle(v,v, LD4_1), Half);
    src.b = cast(shuffle(v,v, LD4_2), Half);
    src.a = cast(shuffle(v,v, LD4_3), Half);
    return src;
}
RGBA store_rgba_f16(void* ptr, RGBA src) {
    F16 r = cast(src.r, F16),
        g = cast(src.g, F16),
        b = cast(src.b, F16),
        a = cast(src.a, F16);
    *(F16x4*)ptr = shuffle(shuffle(r, g, CONCAT),
                           shuffle(b, a, CONCAT), ST4);
    return src;
}

RGBA load_rgb_unorm8(void* ptr, RGBA src) {
    U8x3 v;
    memcpy(&v, ptr, N*3);  // note: sizeof v == N*4!
    src.r = Half_from_U8(shuffle(v,v, LD3_0)) * (half)(1/255.0);
    src.g = Half_from_U8(shuffle(v,v, LD3_1)) * (half)(1/255.0);
    src.b = Half_from_U8(shuffle(v,v, LD3_2)) * (half)(1/255.0);
    src.a = 1;
    return src;
}

RGBA store_rgb_unorm8(void* ptr, RGBA src) {
    U8 r = cast(src.r * 255 + 0.5, U8),
       g = cast(src.g * 255 + 0.5, U8),
       b = cast(src.b * 255 + 0.5, U8);
    *(U8x3*)ptr = shuffle(shuffle(r, g, CONCAT),
                          shuffle(b, b, CONCAT), ST3);
    return src;
}

RGBA load_rgba_unorm8(void* ptr, RGBA src) {
    U8x4 v;
    memcpy(&v, ptr, sizeof v);
    src.r = Half_from_U8(shuffle(v,v, LD4_0)) * (half)(1/255.0);
    src.g = Half_from_U8(shuffle(v,v, LD4_1)) * (half)(1/255.0);
    src.b = Half_from_U8(shuffle(v,v, LD4_2)) * (half)(1/255.0);
    src.a = Half_from_U8(shuffle(v,v, LD4_3)) * (half)(1/255.0);
    return src;
}

RGBA store_rgba_unorm8(void* ptr, RGBA src) {
    U8 r = cast(src.r * 255 + 0.5, U8),
       g = cast(src.g * 255 + 0.5, U8),
       b = cast(src.b * 255 + 0.5, U8),
       a = cast(src.a * 255 + 0.5, U8);
    *(U8x4*)ptr = shuffle(shuffle(r, g, CONCAT),
                          shuffle(b, a, CONCAT), ST4);
    return src;
}

// 0xffff (65535) becomes +inf when converted directly to f16, so unorm16 always goes via f32.
RGBA load_rgba_unorm16(void* ptr, RGBA src) {
    U16x4 v;
    memcpy(&v, ptr, sizeof v);
    src.r = cast( cast(shuffle(v,v, LD4_0), F32) * (1/65535.0f), Half );
    src.g = cast( cast(shuffle(v,v, LD4_1), F32) * (1/65535.0f), Half );
    src.b = cast( cast(shuffle(v,v, LD4_2), F32) * (1/65535.0f), Half );
    src.a = cast( cast(shuffle(v,v, LD4_3), F32) * (1/65535.0f), Half );
    return src;
}

RGBA store_rgba_unorm16(void* ptr, RGBA src) {
    U16 r = cast( cast(src.r, F32) * 65535 + 0.5, U16 ),
        g = cast( cast(src.g, F32) * 65535 + 0.5, U16 ),
        b = cast( cast(src.b, F32) * 65535 + 0.5, U16 ),
        a = cast( cast(src.a, F32) * 65535 + 0.5, U16 );
    *(U16x4*)ptr = shuffle(shuffle(r, g, CONCAT),
                           shuffle(b, a, CONCAT), ST4);
    return src;
}

void drive(Step step[], const int n) {
    RGBA src[1];
    Cold cold;

    int i = 0;
    for (; i+N <= n; i += N) { step->effect(step+1,(size_t)i,*src,&cold); }
    if  (  i   <  n        ) { step->effect(step+1,(size_t)n,*src,&cold); }
}
