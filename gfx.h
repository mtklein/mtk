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

#define vec(T,K)   T __attribute__((vector_size(K*sizeof(T))))
#define splat(v,T) (v)-(T){0}

typedef vec(half, N) Half;
typedef vec(float,N) F32;

typedef struct {
    Half r,g,b,a;
} RGBA;

typedef struct {
    RGBA dst;
    F32  x,y;
} Cold;

typedef RGBA (ABI Effect)(void* ctx, RGBA src, Cold*);
typedef RGBA (ABI Load  )(const void*);
typedef void (ABI Store )(void*, RGBA);

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

void drive(const void*, size_t lbpp, Load*,
           void*,       size_t sbpp, Store*,
           int x, int y, int n,
           Effect* effect[], void* ctx[]);
