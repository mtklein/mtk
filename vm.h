#pragma once

#include <stdint.h>

typedef struct Builder Builder;
typedef struct Program Program;

Builder* builder(void);
Program* compile(Builder*);
void     run    (const Program*, int n, void* arg[]);
void     drop   (Program*);

typedef struct { int ix; } Ptr;

typedef struct { int id; } U8;
typedef struct { int id; } U16;
typedef struct { int id; } U32;

typedef struct { int id; } S8;
typedef struct { int id; } S16;
typedef struct { int id; } S32;

typedef struct { int id; } F16;
typedef struct { int id; } F32;

typedef struct { U8 r,g;     } U8x2;
typedef struct { U8 r,g,b;   } U8x3;
typedef struct { U8 r,g,b,a; } U8x4;

typedef struct { U16 r,g;     } U16x2;
typedef struct { U16 r,g,b;   } U16x3;
typedef struct { U16 r,g,b,a; } U16x4;

typedef struct { U32 r,g;     } U32x2;
typedef struct { U32 r,g,b;   } U32x3;
typedef struct { U32 r,g,b,a; } U32x4;

Ptr arg(Builder*, int stride);

U8   ld1_8(Builder*, Ptr);
U8x2 ld2_8(Builder*, Ptr);
U8x3 ld3_8(Builder*, Ptr);
U8x4 ld4_8(Builder*, Ptr);

U16   ld1_16(Builder*, Ptr);
U16x2 ld2_16(Builder*, Ptr);
U16x3 ld3_16(Builder*, Ptr);
U16x4 ld4_16(Builder*, Ptr);

U32   ld1_32(Builder*, Ptr);
U32x2 ld2_32(Builder*, Ptr);
U32x3 ld3_32(Builder*, Ptr);
U32x4 ld4_32(Builder*, Ptr);

void st1_8(Builder*, Ptr, U8);
void st2_8(Builder*, Ptr, U8x2);
void st3_8(Builder*, Ptr, U8x3);
void st4_8(Builder*, Ptr, U8x4);

void st1_16(Builder*, Ptr, U16);
void st2_16(Builder*, Ptr, U16x2);
void st3_16(Builder*, Ptr, U16x3);
void st4_16(Builder*, Ptr, U16x4);

void st1_32(Builder*, Ptr, U32);
void st2_32(Builder*, Ptr, U32x2);
void st3_32(Builder*, Ptr, U32x3);
void st4_32(Builder*, Ptr, U32x4);

U8  splat_U8 (Builder*, uint8_t);
U16 splat_U16(Builder*, uint16_t);
U32 splat_U32(Builder*, uint32_t);

S8  splat_S8 (Builder*, int8_t);
S16 splat_S16(Builder*, int16_t);
S32 splat_S32(Builder*, int32_t);

F16 splat_F16(Builder*, _Float16);
F32 splat_F32(Builder*, float);

F16 cast_F16_from_U16(Builder*, U16);
U16 cast_U16_from_F16(Builder*, F16);

F16 cast_F16_from_S16(Builder*, S16);
S16 cast_S16_from_F16(Builder*, F16);

F32 cast_F32_from_U32(Builder*, U32);
U32 cast_U32_from_F32(Builder*, F32);

F32 cast_F32_from_S32(Builder*, S32);
S32 cast_S32_from_F32(Builder*, F32);

U16 widen_U8 (Builder*, U8);
S16 widen_S8 (Builder*, S8);
U32 widen_U16(Builder*, U16);
S32 widen_S16(Builder*, S16);
F32 widen_F16(Builder*, F16);

U8  narrow_U16(Builder*, U16);
S8  narrow_S16(Builder*, S16);
U16 narrow_U32(Builder*, U32);
S16 narrow_S32(Builder*, S32);
F16 narrow_F32(Builder*, F32);

F16 add_F16(Builder*, F16, F16);
F16 sub_F16(Builder*, F16, F16);
F16 mul_F16(Builder*, F16, F16);
F16 div_F16(Builder*, F16, F16);
