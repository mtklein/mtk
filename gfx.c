#include "gfx.h"
#include <stdint.h>

const Affine identity = {
    1,0,0,
    0,1,0,
};

void shade_color(void* ctx, Color* c, Point p) {
    (void)p;
    *c = *(Color*)ctx;
}

void blend_src(Color* src, const Color* dst) {
    (void)src;
    (void)dst;
}

void blend_dst(Color* src, const Color* dst) {
    *src = *dst;
}

void blend_srcover(Color* src, const Color* dst) {
    src->r += dst->r * (1 - src->a);
    src->g += dst->g * (1 - src->a);
    src->b += dst->b * (1 - src->a);
    src->a += dst->a * (1 - src->a);
}

void pixel_rgba_fp16(void* px, const Color* src, Color* dst) {
    struct { _Float16 r,g,b,a; } *p = px;
    if (src) {
        p->r = src->r;
        p->g = src->g;
        p->b = src->b;
        p->a = src->a;
    } else {
        dst->r = p->r;
        dst->g = p->g;
        dst->b = p->b;
        dst->a = p->a;
    }
}

void pixel_rgba_fp32(void* px, const Color* src, Color* dst) {
    struct { float r,g,b,a; } *p = px;
    if (src) {
        p->r = (float)src->r;
        p->g = (float)src->g;
        p->b = (float)src->b;
        p->a = (float)src->a;
    } else {
        dst->r = (_Float16)p->r;
        dst->g = (_Float16)p->g;
        dst->b = (_Float16)p->b;
        dst->a = (_Float16)p->a;
    }
}

void pixel_rgba_unorm8(void* px, const Color* src, Color* dst) {
    // TODO: codegen here is not very good... byte aliasing perhaps?
    struct { uint8_t r,g,b,a; } *p = px;
    if (src) {
        p->r = (uint8_t)( src->r * 255.0f16 + 0.5f16 );
        p->g = (uint8_t)( src->g * 255.0f16 + 0.5f16 );
        p->b = (uint8_t)( src->b * 255.0f16 + 0.5f16 );
        p->a = (uint8_t)( src->a * 255.0f16 + 0.5f16 );
    } else {
        dst->r = p->r * (1/255.0f16);
        dst->g = p->g * (1/255.0f16);
        dst->b = p->b * (1/255.0f16);
        dst->a = p->a * (1/255.0f16);
    }
}

void pixel_rgba_unorm16(void* px, const Color* src, Color* dst) {
    // 65535 is too big for _Float16, so load and store via float.
    // TODO: may be a more direct conversion possible?
    struct { uint16_t r,g,b,a; } *p = px;
    if (src) {
        p->r = (uint16_t)( (float)src->r * 65535.0f + 0.5f );
        p->g = (uint16_t)( (float)src->g * 65535.0f + 0.5f );
        p->b = (uint16_t)( (float)src->b * 65535.0f + 0.5f );
        p->a = (uint16_t)( (float)src->a * 65535.0f + 0.5f );
    } else {
        dst->r = (_Float16)( p->r * (1/65535.0f) );
        dst->g = (_Float16)( p->g * (1/65535.0f) );
        dst->b = (_Float16)( p->b * (1/65535.0f) );
        dst->a = (_Float16)( p->a * (1/65535.0f) );
    }
}

void pixel_rgba_1010102(void* px, const Color* src, Color* dst) {
    struct {
        uint32_t r : 10;
        uint32_t g : 10;
        uint32_t b : 10;
        uint32_t a :  2;
    } *p = px;
    if (src) {
        p->r = (uint32_t)( src->r * 1023.0f16 + 0.5f16 );
        p->g = (uint32_t)( src->g * 1023.0f16 + 0.5f16 );
        p->b = (uint32_t)( src->b * 1023.0f16 + 0.5f16 );
        p->a = (uint32_t)( src->a *    3.0f16 + 0.5f16 );
    } else {
        dst->r = (_Float16)( p->r * (1/1023.0f16) );
        dst->g = (_Float16)( p->g * (1/1023.0f16) );
        dst->b = (_Float16)( p->b * (1/1023.0f16) );
        dst->a = (_Float16)( p->a * (1/   3.0f16) );
    }
}

void pixel_rgba_101010x(void* px, const Color* src, Color* dst) {
    struct {
        uint32_t r : 10;
        uint32_t g : 10;
        uint32_t b : 10;
        uint32_t x :  2;
    } *p = px;
    if (src) {
        p->r = (uint32_t)( src->r * 1023.0f16 + 0.5f16 );
        p->g = (uint32_t)( src->g * 1023.0f16 + 0.5f16 );
        p->b = (uint32_t)( src->b * 1023.0f16 + 0.5f16 );
        p->x = 3;
    } else {
        dst->r = (_Float16)( p->r * (1/1023.0f16) );
        dst->g = (_Float16)( p->g * (1/1023.0f16) );
        dst->b = (_Float16)( p->b * (1/1023.0f16) );
        dst->a = 1.0f16;
    }
}
