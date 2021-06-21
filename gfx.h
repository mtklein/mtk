#pragma once

#define N 8
typedef _Float16 half;
typedef half  __attribute__((ext_vector_type(N))) Half;
typedef float __attribute__((ext_vector_type(N))) Float;

typedef struct { float r,g,b,a; } Color;
typedef struct { Half  r,g,b,a; } Slab;

typedef Slab (Shade)(void *ctx, Float x, Float y);
typedef Slab (Blend)(Slab src, Slab dst);
typedef Slab (Load )(const void *px);
typedef void (Store)(void *px, Slab);

Shade
    shade_color;

Blend
    blend_src,
    blend_dst,
    blend_srcover;

Load
    load_rgba_f16,
    load_rgba_f32,
    load_rgba_unorm8,
    load_rgba_unorm16,
    load_rgba_1010102;

Store
    store_rgba_f16,
    store_rgba_f32,
    store_rgba_unorm8,
    store_rgba_unorm16,
    store_rgba_1010102;
