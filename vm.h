#pragma once

#include <stdint.h>

typedef struct Builder Builder;
typedef struct Program Program;

Builder* builder(void);
Program* compile(Builder*);
void     run    (const Program*, int n, void* arg[]);
void     drop   (Program*);

typedef struct { int ix; } Ptr;

// TODO struct { int id : 30; int sub : 2; } for U??xN support

typedef struct { int id; } U8;
typedef struct { int id; } S8;

typedef struct { int id; } U16;
typedef struct { int id; } S16;
typedef struct { int id; } F16;

typedef struct { int id; } U32;
typedef struct { int id; } S32;
typedef struct { int id; } F32;

typedef struct { U8 r,g;     } U8x2;
typedef struct { U8 r,g,b;   } U8x3;
typedef struct { U8 r,g,b,a; } U8x4;

typedef struct { S8 r,g;     } S8x2;
typedef struct { S8 r,g,b;   } S8x3;
typedef struct { S8 r,g,b,a; } S8x4;

typedef struct { U16 r,g;     } U16x2;
typedef struct { U16 r,g,b;   } U16x3;
typedef struct { U16 r,g,b,a; } U16x4;

typedef struct { S16 r,g;     } S16x2;
typedef struct { S16 r,g,b;   } S16x3;
typedef struct { S16 r,g,b,a; } S16x4;

typedef struct { F16 r,g;     } F16x2;
typedef struct { F16 r,g,b;   } F16x3;
typedef struct { F16 r,g,b,a; } F16x4;

typedef struct { U32 r,g;     } U32x2;
typedef struct { U32 r,g,b;   } U32x3;
typedef struct { U32 r,g,b,a; } U32x4;

typedef struct { S32 r,g;     } S32x2;
typedef struct { S32 r,g,b;   } S32x3;
typedef struct { S32 r,g,b,a; } S32x4;

typedef struct { F32 r,g;     } F32x2;
typedef struct { F32 r,g,b;   } F32x3;
typedef struct { F32 r,g,b,a; } F32x4;

Ptr arg(Builder*, int stride);

U8   ld1_U8(Builder*, Ptr);
U8x2 ld2_U8(Builder*, Ptr);
U8x3 ld3_U8(Builder*, Ptr);
U8x4 ld4_U8(Builder*, Ptr);

S8   ld1_S8(Builder*, Ptr);
S8x2 ld2_S8(Builder*, Ptr);
S8x3 ld3_S8(Builder*, Ptr);
S8x4 ld4_S8(Builder*, Ptr);

U16   ld1_U16(Builder*, Ptr);
U16x2 ld2_U16(Builder*, Ptr);
U16x3 ld3_U16(Builder*, Ptr);
U16x4 ld4_U16(Builder*, Ptr);

S16   ld1_S16(Builder*, Ptr);
S16x2 ld2_S16(Builder*, Ptr);
S16x3 ld3_S16(Builder*, Ptr);
S16x4 ld4_S16(Builder*, Ptr);

F16   ld1_F16(Builder*, Ptr);
F16x2 ld2_F16(Builder*, Ptr);
F16x3 ld3_F16(Builder*, Ptr);
F16x4 ld4_F16(Builder*, Ptr);

U32   ld1_U32(Builder*, Ptr);
U32x2 ld2_U32(Builder*, Ptr);
U32x3 ld3_U32(Builder*, Ptr);
U32x4 ld4_U32(Builder*, Ptr);

S32   ld1_S32(Builder*, Ptr);
S32x2 ld2_S32(Builder*, Ptr);
S32x3 ld3_S32(Builder*, Ptr);
S32x4 ld4_S32(Builder*, Ptr);

F32   ld1_F32(Builder*, Ptr);
F32x2 ld2_F32(Builder*, Ptr);
F32x3 ld3_F32(Builder*, Ptr);
F32x4 ld4_F32(Builder*, Ptr);

void st1_U8(Builder*, Ptr, U8);
void st2_U8(Builder*, Ptr, U8x2);
void st3_U8(Builder*, Ptr, U8x3);
void st4_U8(Builder*, Ptr, U8x4);

void st1_S8(Builder*, Ptr, S8);
void st2_S8(Builder*, Ptr, S8x2);
void st3_S8(Builder*, Ptr, S8x3);
void st4_S8(Builder*, Ptr, S8x4);

void st1_U16(Builder*, Ptr, U16);
void st2_U16(Builder*, Ptr, U16x2);
void st3_U16(Builder*, Ptr, U16x3);
void st4_U16(Builder*, Ptr, U16x4);

void st1_S16(Builder*, Ptr, S16);
void st2_S16(Builder*, Ptr, S16x2);
void st3_S16(Builder*, Ptr, S16x3);
void st4_S16(Builder*, Ptr, S16x4);

void st1_F16(Builder*, Ptr, F16);
void st2_F16(Builder*, Ptr, F16x2);
void st3_F16(Builder*, Ptr, F16x3);
void st4_F16(Builder*, Ptr, F16x4);

void st1_U32(Builder*, Ptr, U32);
void st2_U32(Builder*, Ptr, U32x2);
void st3_U32(Builder*, Ptr, U32x3);
void st4_U32(Builder*, Ptr, U32x4);

void st1_S32(Builder*, Ptr, S32);
void st2_S32(Builder*, Ptr, S32x2);
void st3_S32(Builder*, Ptr, S32x3);
void st4_S32(Builder*, Ptr, S32x4);

void st1_F32(Builder*, Ptr, F32);
void st2_F32(Builder*, Ptr, F32x2);
void st3_F32(Builder*, Ptr, F32x3);
void st4_F32(Builder*, Ptr, F32x4);

U8  splat_U8 (Builder*, uint8_t);
S8  splat_S8 (Builder*, int8_t);

U16 splat_U16(Builder*, uint16_t);
S16 splat_S16(Builder*, int16_t);
F16 splat_F16(Builder*, _Float16);

U32 splat_U32(Builder*, uint32_t);
S32 splat_S32(Builder*, int32_t);
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
