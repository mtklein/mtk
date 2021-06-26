#pragma once

#include <stddef.h>
#include <stdint.h>

#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    #define N 8
    typedef _Float16 half;
    typedef int16_t  mask;
#elif defined(__AVX__)
    #define N 8
    typedef float half;
    typedef int   mask;
#else
    #define N 4
    typedef float half;
    typedef int   mask;
#endif

typedef half  __attribute__((ext_vector_type(N))) Half;
typedef float __attribute__((ext_vector_type(N))) F32;

typedef struct {
    Half r,g,b,a;
} Slab;

typedef struct {
    Slab dst;
    F32 x,y;
} Cold;

typedef Slab (Effect)(void* ctx, Slab src, Cold*);
typedef Slab (Load  )(const void*);
typedef void (Store )(void*, Slab);

typedef struct { float x,y; }     Point;
typedef struct { float r,g,b,a; } Color;
typedef struct { float sx,kx,tx,
                       ky,sy,ty,
                       p0,p1,p2; } Matrix;

Effect
    apply_affine_Matrix,
    apply_perspective_Matrix,
    blend_dst,
    blend_src,
    blend_srcover,
    clamp_01,
    shade_Color;

Load
    load_rgb_unorm8,
    load_rgba_f16,
    load_rgba_unorm16,
    load_rgba_unorm8;

Store
    store_rgb_unorm8,
    store_rgba_f16,
    store_rgba_unorm16,
    store_rgba_unorm8;

void drive(void* dptr, int n,
           int x, int y,
           Load*, Store*, size_t bpp,
           Effect* effect[], void* ctx[]);
