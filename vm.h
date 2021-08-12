#pragma once

typedef struct Builder Builder;
typedef struct Program Program;

Builder* builder(void);
Program* compile(Builder*);
void     run    (const Program*, int n, void* arg[]);
void     drop   (Program*);

typedef struct { int ix; } Ptr;
Ptr arg(Builder*, int stride);

typedef struct { int id; } V8;
typedef struct { int id; } V16;
typedef struct { int id; } V32;

V8                          ld1_8(Builder*, Ptr);
struct V8x2 { V8 r,g    ; } ld2_8(Builder*, Ptr);
struct V8x3 { V8 r,g,b  ; } ld3_8(Builder*, Ptr);
struct V8x4 { V8 r,g,b,a; } ld4_8(Builder*, Ptr);

V16                           ld1_16(Builder*, Ptr);
struct V16x2 { V16 r,g    ; } ld2_16(Builder*, Ptr);
struct V16x3 { V16 r,g,b  ; } ld3_16(Builder*, Ptr);
struct V16x4 { V16 r,g,b,a; } ld4_16(Builder*, Ptr);

V32                           ld1_32(Builder*, Ptr);
struct V32x2 { V32 r,g    ; } ld2_32(Builder*, Ptr);
struct V32x3 { V32 r,g,b  ; } ld3_32(Builder*, Ptr);
struct V32x4 { V32 r,g,b,a; } ld4_32(Builder*, Ptr);

void st1_8(Builder*, Ptr, V8);
void st2_8(Builder*, Ptr, V8,V8);
void st3_8(Builder*, Ptr, V8,V8,V8);
void st4_8(Builder*, Ptr, V8,V8,V8,V8);

void st1_16(Builder*, Ptr, V16);
void st2_16(Builder*, Ptr, V16,V16);
void st3_16(Builder*, Ptr, V16,V16,V16);
void st4_16(Builder*, Ptr, V16,V16,V16,V16);

void st1_32(Builder*, Ptr, V32);
void st2_32(Builder*, Ptr, V32,V32);
void st3_32(Builder*, Ptr, V32,V32,V32);
void st4_32(Builder*, Ptr, V32,V32,V32,V32);

V8  splat_8 (Builder*, int);
V16 splat_16(Builder*, int);
V32 splat_32(Builder*, int);

V8  uniform_8 (Builder*, Ptr, int offset);
V16 uniform_16(Builder*, Ptr, int offset);
V32 uniform_32(Builder*, Ptr, int offset);

V16 cast_F16_to_S16(Builder*, V16);
V16 cast_F16_to_U16(Builder*, V16);
V16 cast_S16_to_F16(Builder*, V16);
V16 cast_U16_to_F16(Builder*, V16);

V32 cast_F32_to_S32(Builder*, V32);
V32 cast_F32_to_U32(Builder*, V32);
V32 cast_S32_to_F32(Builder*, V32);
V32 cast_U32_to_F32(Builder*, V32);

V16 widen_S8 (Builder*, V8);
V16 widen_U8 (Builder*, V8);
V32 widen_F16(Builder*, V16);
V32 widen_S16(Builder*, V16);
V32 widen_U16(Builder*, V16);

V16 narrow_F32(Builder*, V32);
V16 narrow_I32(Builder*, V32);
V8  narrow_I16(Builder*, V16);

V16  add_F16(Builder*, V16, V16);
V16  sub_F16(Builder*, V16, V16);
V16  mul_F16(Builder*, V16, V16);
V16  div_F16(Builder*, V16, V16);
V16 sqrt_F16(Builder*, V16);

V32 add_I32(Builder*, V32, V32);
V32 sub_I32(Builder*, V32, V32);
V32 mul_I32(Builder*, V32, V32);

V32 shl_I32(Builder*, V32, int);
V32 shr_S32(Builder*, V32, int);
V32 shr_U32(Builder*, V32, int);
