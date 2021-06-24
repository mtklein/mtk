#pragma once

#define N 8
typedef _Float16 __attribute__((ext_vector_type(N))) F16;
typedef float    __attribute__((ext_vector_type(N))) F32;

typedef struct { F16 r,g,b,a; } Slab;
typedef Slab (Effect)(void *ctx, Slab src, Slab dst, F32 x, F32 y);

typedef struct { float x,y; }     Point;
typedef struct { float r,g,b,a; } Color;

extern const struct Shade_Color {
    Effect* effect;
    Color   color;
} shade_color_init;

extern const struct Shade_Gradient_2pt {
    Effect* effect;
    Color   cA,cB;
    Point   pA,pB;
} shade_gradient_2pt_init;

Effect
    blend_src,
    blend_dst,
    blend_srcover,
    load_rgba_f16,
    load_rgba_unorm8,
    load_rgba_unorm16,
    store_rgba_f16,
    store_rgba_unorm8,
    store_rgba_unorm16;
