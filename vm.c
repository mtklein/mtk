#include "array.h"
#include "vm.h"
#include <stdlib.h>
#include <string.h>

#define N 16

typedef union {
    uint8_t  u8;
    uint16_t u16;
    uint32_t u32;
     int8_t  s8;
     int16_t s16;
     int32_t s32;
    _Float16 f16;
    float    f32;
} Imm;

typedef union {
    uint8_t  __attribute__((vector_size(1*N))) u8;
    uint16_t __attribute__((vector_size(2*N))) u16;
    uint32_t __attribute__((vector_size(4*N))) u32;
     int8_t  __attribute__((vector_size(1*N))) s8;
     int16_t __attribute__((vector_size(2*N))) s16;
     int32_t __attribute__((vector_size(4*N))) s32;
    _Float16 __attribute__((vector_size(2*N))) f16;
    float    __attribute__((vector_size(4*N))) f32;
} Val;

typedef struct Inst {
    void (*op1)(const Program*, const struct Inst*, Val val[], void* arg[]);
    void (*opN)(const Program*, const struct Inst*, Val val[], void* arg[]);
    int x,y,z,w;
    Ptr ptr;
    Imm imm;
} Inst;

struct Builder {
    Inst* inst;
    int*  stride;
    int   insts;
    int   args;
};

struct Program {
    Inst* inst;
    int*  stride;
    int   insts;
    int   args;
};

Builder* builder() {
    Builder* b = calloc(1, sizeof *b);
    return b;
}

static void op1_done(const Program* p, const Inst* inst, Val val[], void* arg[]) {
    (void)inst;
    (void)val;
    for (int i = 0; i < p->args; i++) {
        arg[i] = (char*)arg[i] + 1*p->stride[i];
    }
}
static void opN_done(const Program* p, const Inst* inst, Val val[], void* arg[]) {
    (void)inst;
    (void)val;
    for (int i = 0; i < p->args; i++) {
        arg[i] = (char*)arg[i] + N*p->stride[i];
    }
}

static void op1_done1(const Program* p, const Inst* inst, Val val[], void* arg[]) {
    (void)p;
    (void)inst;
    (void)val;
    arg[0] = (char*)arg[0] + 1*p->stride[0];
}
static void opN_done1(const Program* p, const Inst* inst, Val val[], void* arg[]) {
    (void)p;
    (void)inst;
    (void)val;
    arg[0] = (char*)arg[0] + N*p->stride[0];
}

Program* compile(Builder* b) {
    switch (b->args) {
        default: push(b->inst,b->insts) = (Inst) {.op1 = op1_done , .opN = opN_done }; break;
        case 1:  push(b->inst,b->insts) = (Inst) {.op1 = op1_done1, .opN = opN_done1}; break;
    }

    Program* p = calloc(1, sizeof *p);
    p->inst   = b->inst;
    p->stride = b->stride;
    p->insts  = b->insts;
    p->args   = b->args;

    free(b);
    return p;
}

void drop(Program* p) {
    free(p->inst);
    free(p->stride);
    free(p);
}

Ptr arg(Builder* b, int stride) {
    int ix = b->args;
    push(b->stride,b->args) = stride;
    return (Ptr){ix};
}

#define next1 inst++; inst->op1(p,inst,val,arg)
#define nextN inst++; inst->opN(p,inst,val,arg)

static void op1_splat_32(const Program* p, const Inst* inst, Val val[], void* arg[]) {
    Val v = {0};
    v.u32 += inst->imm.u32;
    val[inst - p->inst] = v;
    next1;
}
static void opN_splat_32(const Program* p, const Inst* inst, Val val[], void* arg[]) {
    Val v = {0};
    v.u32 += inst->imm.u32;
    val[inst - p->inst] = v;
    nextN;
}
U32 splat_U32(Builder* b, uint32_t imm) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst) {
        .op1     = op1_splat_32,
        .opN     = opN_splat_32,
        .imm.u32 = imm,
    };
    return (U32){id};
}

static void op1_st1_32(const Program* p, const Inst* inst, Val val[], void* arg[]) {
    memcpy(arg[inst->ptr.ix], &val[inst->x].u32, 1*4);
    next1;
}
static void opN_st1_32(const Program* p, const Inst* inst, Val val[], void* arg[]) {
    memcpy(arg[inst->ptr.ix], &val[inst->x].u32, N*4);
    nextN;
}
void st1_32(Builder* b, Ptr p, U32 v) {
    push(b->inst,b->insts) = (Inst) {
        .op1 = op1_st1_32,
        .opN = opN_st1_32,
        .ptr = p,
        .x   = v.id,
    };
}

void run(const Program* p, int n, void* arg[]) {
    Val* val = malloc((size_t)p->insts * sizeof *val);

    for (; n >= N; n -= N) { p->inst->opN(p,p->inst,val,arg); }
    while (n --> 0)        { p->inst->op1(p,p->inst,val,arg); }

    free(val);
}
