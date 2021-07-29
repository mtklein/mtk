#pragma once

#include <stdint.h>

typedef enum {
     x0, x1, x2, x3, x4, x5, x6, x7, x8, x9,x10,x11,x12,x13,x14,x15,
    x16,x17,x18,x19,x20,x21,x22,x23,x24,x25,x26,x27,x28,x29,x30,xzr,
    lr=x30,
    sp=xzr,
} X;

typedef enum {
     v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,v10,v11,v12,v13,v14,v15,
    v16,v17,v18,v19,v20,v21,v22,v23,v24,v25,v26,v27,v28,v29,v30,v31,
} V;

typedef enum { _8b,_16b, _4h,_8h, _2s,_4s, /*no _1d*/ _2d=7 } Arr;

uint32_t xret(X n);

uint32_t xadd(X d, X n, unsigned imm12);
uint32_t xsub(X d, X n, unsigned imm12);

uint32_t vnot(Arr, V d, V n);
uint32_t vand(Arr, V d, V n, V m);
uint32_t vorr(Arr, V d, V n, V m);
uint32_t veor(Arr, V d, V n, V m);
uint32_t vbic(Arr, V d, V n, V m);
uint32_t vbsl(Arr, V d, V n, V m);

uint32_t vadd(Arr, V d, V n, V m);
uint32_t vsub(Arr, V d, V n, V m);
uint32_t vmul(Arr, V d, V n, V m);

uint32_t vshl (Arr, V d, V n, int imm);
uint32_t vsshr(Arr, V d, V n, int imm);
uint32_t vushr(Arr, V d, V n, int imm);
uint32_t vcmeq(Arr, V d, V n, V m);
uint32_t vcmgt(Arr, V d, V n, V m);

uint32_t vfadd (Arr, V d, V n, V m);
uint32_t vfsub (Arr, V d, V n, V m);
uint32_t vfmul (Arr, V d, V n, V m);
uint32_t vfdiv (Arr, V d, V n, V m);
uint32_t vfmin (Arr, V d, V n, V m);
uint32_t vfmax (Arr, V d, V n, V m);
uint32_t vfmla (Arr, V d, V n, V m);
uint32_t vfmls (Arr, V d, V n, V m);
uint32_t vfcmeq(Arr, V d, V n, V m);
uint32_t vfcmgt(Arr, V d, V n, V m);
uint32_t vfcmge(Arr, V d, V n, V m);
