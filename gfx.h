#pragma once

typedef struct {
    float x,y;
} Point;

typedef struct {
    float sx,kx,tx,
          ky,sy,ty;
} Affine;

typedef struct {
    _Float16 r,g,b,a;
} Color;

typedef void (Shade)(void*, Color*, Point);
typedef void (Blend)(Color*, const Color*);
typedef void (Pixel)(void*, const Color*, Color*);

typedef struct {
    void* pixels;
    int   w,h,bpp,row;
} Bitmap;

typedef struct {
    Affine src2dst;
    Shade* shade; void* shade_ctx;
    Blend* blend;
    Pixel* pixel;
} Draw;


extern const Affine identity;

Shade shade_color;

Blend blend_src,
      blend_dst,
      blend_srcover;

Pixel pixel_rgba_fp16,
      pixel_rgba_fp32,
      pixel_rgba_unorm8,
      pixel_rgba_unorm16,
      pixel_rgba_1010102,
      pixel_rgba_101010x;
