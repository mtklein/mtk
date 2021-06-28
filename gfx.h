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
} RGBA;

typedef struct {
    RGBA dst;
    F32  x,y;
} Cold;

typedef RGBA (ABI Effect)(void* ctx, int i, RGBA src, Cold*);

Effect
    blend_dst,
    blend_src,
    blend_srcover,
    clamp_01,
    load1_rgb_unorm8,
    load1_rgba_f16,
    load1_rgba_unorm16,
    load1_rgba_unorm8,
    loadN_rgb_unorm8,
    loadN_rgba_f16,
    loadN_rgba_unorm16,
    loadN_rgba_unorm8,
    matrix_2x3,
    matrix_3x3,
    shade_rgba_f32,
    store1_rgb_unorm8,
    store1_rgba_f16,
    store1_rgba_unorm16,
    store1_rgba_unorm8,
    storeN_rgb_unorm8,
    storeN_rgba_f16,
    storeN_rgba_unorm16,
    storeN_rgba_unorm8;

typedef struct {
    Effect *effectN,
           *effect1;
    void   *ctx;
} Step;

void drive(const Step step[], int n, int x, int y);
