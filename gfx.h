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

union Step;

typedef RGBA (ABI Effect)(union Step* step, size_t p, RGBA src, Cold*);
typedef RGBA (ABI Load  )(const void*);
typedef RGBA (ABI Store )(void*, RGBA);

typedef union Step {
    Effect* effect;
    Load*   load;
    Store*  store;
    void*   ptr;
    size_t  size;
} Step;

Effect
    blend_dst,
    blend_src,
    blend_srcover,
    clamp_01,
    done,
    load,
    matrix_2x3,
    matrix_3x3,
    seed_xy,
    shade_rgba_f32,
    store;

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

void drive(Step step[], int n);
