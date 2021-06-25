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

    #define CONCAT  0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15

    #define ST3  0, 8,16,    1, 9,17,    2,10,18,    3,11,19,    \
                 4,12,20,    5,13,21,    6,14,22,    7,15,23
    #define ST4  0, 8,16,24, 1, 9,17,25, 2,10,18,26, 3,11,19,27, \
                 4,12,20,28, 5,13,21,29, 6,14,22,30, 7,15,23,31
#endif

typedef uint8_t __attribute__((ext_vector_type(  N))) U8;
typedef uint8_t __attribute__((ext_vector_type(3*N))) U8x3;
typedef uint8_t __attribute__((ext_vector_type(4*N))) U8x4;

typedef int16_t __attribute__((ext_vector_type(  N))) S16;

typedef uint16_t __attribute__((ext_vector_type(  N))) U16;
typedef uint16_t __attribute__((ext_vector_type(4*N))) U16x4;

typedef _Float16 __attribute__((ext_vector_type(4*N))) F16x4;

typedef _Float16 __attribute__((ext_vector_type(4))) Half4;
typedef float    __attribute__((ext_vector_type(4))) Float4;

static F16 select(S16 cond, F16 t, F16 e) { return (F16)( ( cond & (S16)t)
                                                        | (~cond & (S16)e) ); }
static F16 min(F16 x, F16 y) { return select(y < x, y, x); }
static F16 max(F16 x, F16 y) { return select(x < y, y, x); }
static F16 clamp(F16 x, F16 lo, F16 hi) { return max(lo, min(x, hi)); }

// LLVM won't use uvcvf.8h to convert U16 to F16, otherwise we'd just cast(u8, F16).
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

Slab apply_affine_Matrix(void* ctx, Slab src, Cold* cold) {
    const Matrix* m = ctx;
    F32 x = cold->x * m->sx + (cold->y * m->kx + m->tx),
        y = cold->x * m->ky + (cold->y * m->sy + m->ty);
    cold->x = x;
    cold->y = y;
    return src;
}

Slab apply_perspective_Matrix(void* ctx, Slab src, Cold* cold) {
    const Matrix* m = ctx;
    F32 x = cold->x * m->sx + (cold->y * m->kx + m->tx),
        y = cold->x * m->ky + (cold->y * m->sy + m->ty),
        z = cold->x * m->p0 + (cold->y * m->p1 + m->p2);
    cold->x = x * (1/z);
    cold->y = y * (1/z);
    return src;
}

Slab shade_Color(void* ctx, Slab src, Cold* cold) {
    (void)cold;
    const Color* color = ctx;
    Half4 rgba = cast((Float4){
        color->r,
        color->g,
        color->b,
        color->a,
    }, Half4);
    src.r = rgba.r;
    src.g = rgba.g;
    src.b = rgba.b;
    src.a = rgba.a;
    return src;
}

Slab blend_src(void* ctx, Slab src, Cold* cold) {
    (void)ctx;
    (void)cold;
    return src;
}

Slab blend_dst(void* ctx, Slab src, Cold* cold) {
    (void)ctx;
    src = cold->dst;
    return src;
}

Slab blend_srcover(void* ctx, Slab src, Cold* cold) {
    (void)ctx;
    src.r += cold->dst.r * (1-src.a);
    src.g += cold->dst.g * (1-src.a);
    src.b += cold->dst.b * (1-src.a);
    src.a += cold->dst.a * (1-src.a);
    return src;
}

Slab clamp_01(void* ctx, Slab src, Cold* cold) {
    (void)ctx;
    (void)cold;
    src.r = clamp(src.r, 0.0f16, 1.0f16);
    src.g = clamp(src.g, 0.0f16, 1.0f16);
    src.b = clamp(src.b, 0.0f16, 1.0f16);
    src.a = clamp(src.a, 0.0f16, 1.0f16);
    return src;
}

Slab load_rgba_f16(const void* ptr) {
    F16x4 v = *(const F16x4*)ptr;
    return (Slab) {
        shuffle(v,v, LD4_0),
        shuffle(v,v, LD4_1),
        shuffle(v,v, LD4_2),
        shuffle(v,v, LD4_3),
    };
}

void store_rgba_f16(void* ptr, Slab src) {
    *(F16x4*)ptr = shuffle(shuffle(src.r, src.g, CONCAT),
                           shuffle(src.b, src.a, CONCAT), ST4);
}

Slab load_rgb_unorm8(const void* ptr) {
    U8x3 v = *(const U8x3*)ptr;
    return (Slab) {
        F16_from_U8(shuffle(v,v, LD3_0)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD3_1)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD3_2)) * (1/255.0f16),
        1.0f16,
    };
}

void store_rgb_unorm8(void* ptr, Slab src) {
    U8 r = cast(src.r * 255.0f16 + 0.5f16, U8),
       g = cast(src.g * 255.0f16 + 0.5f16, U8),
       b = cast(src.b * 255.0f16 + 0.5f16, U8);
    *(U8x3*)ptr = shuffle(shuffle(r, g, CONCAT),
                          shuffle(b, b, CONCAT), ST3);
}

Slab load_rgba_unorm8(const void* ptr) {
    U8x4 v = *(const U8x4*)ptr;
    return (Slab) {
        F16_from_U8(shuffle(v,v, LD4_0)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD4_1)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD4_2)) * (1/255.0f16),
        F16_from_U8(shuffle(v,v, LD4_3)) * (1/255.0f16),
    };
}

void store_rgba_unorm8(void* ptr, Slab src) {
    U8 r = cast(src.r * 255.0f16 + 0.5f16, U8),
       g = cast(src.g * 255.0f16 + 0.5f16, U8),
       b = cast(src.b * 255.0f16 + 0.5f16, U8),
       a = cast(src.a * 255.0f16 + 0.5f16, U8);
    *(U8x4*)ptr = shuffle(shuffle(r, g, CONCAT),
                          shuffle(b, a, CONCAT), ST4);
}

// 0xffff (65535) becomes +inf when converted directly to f16, so this goes via f32.
Slab load_rgba_unorm16(const void* ptr) {
    U16x4 v = *(const U16x4*)ptr;
    return (Slab) {
        cast( cast(shuffle(v,v, LD4_0), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, LD4_1), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, LD4_2), F32) * (1/65535.0f), F16 ),
        cast( cast(shuffle(v,v, LD4_3), F32) * (1/65535.0f), F16 ),
    };
}

void store_rgba_unorm16(void* ptr, Slab src) {
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
    Slab src  = {0};
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
