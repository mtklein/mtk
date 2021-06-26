#pragma once

#include <stddef.h>

#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    #define N 8
    typedef _Float16 half;
#elif defined(__AVX__)
    #define N 8
    typedef float half;
#else
    #define N 4
    typedef float half;
#endif

#if defined(__x86_64__)
    #define ABI __regcall
#else
    #define ABI
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

typedef Slab (ABI Effect)(void* ctx, Slab src, Cold*);
typedef Slab (ABI Load  )(const void*);
typedef void (ABI Store )(void*, Slab);

Effect
    blend_dst,
    blend_src,
    blend_srcover,
    clamp_01,
    matrix_2x3,
    matrix_3x3,
    shade_rgba_f32;

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
