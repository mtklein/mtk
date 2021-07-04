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
    void (*op1)(const struct Inst*, const struct Inst*, Val val[], void* arg[]);
    void (*opN)(const struct Inst*, const struct Inst*, Val val[], void* arg[]);
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

static void op_done(const Inst* program, const Inst* inst, Val val[], void* arg[]) {
    (void)program;
    (void)inst;
    (void)val;
    (void)arg;
}

Program* compile(Builder* b) {
    push(b->inst,b->insts) = (Inst) {
        .op1 = op_done,
        .opN = op_done,
    };

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

#define next1 inst++; inst->op1(program, inst, val, arg)
#define nextN inst++; inst->opN(program, inst, val, arg)

static void op1_splat_32(const Inst* program, const Inst* inst, Val val[], void* arg[]) {
    Val v = {0};
    v.u32 += inst->imm.u32;
    val[inst-program] = v;
    next1;
}
static void opN_splat_32(const Inst* program, const Inst* inst, Val val[], void* arg[]) {
    Val v = {0};
    v.u32 += inst->imm.u32;
    val[inst-program] = v;
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

static void op1_st1_32(const Inst* program, const Inst* inst, Val val[], void* arg[]) {
    memcpy(arg[inst->ptr.ix], &val[inst->x].u32, 1*4);
    next1;
}
static void opN_st1_32(const Inst* program, const Inst* inst, Val val[], void* arg[]) {
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
    Val* val = calloc((size_t)p->insts, sizeof *val);

    for (; n >= N; n -= N) {
        const Inst* program = p->inst;
        program->opN(program,program,val,arg);
        for (int i = 0; i < p->args; i++) {
            arg[i] = (char*)arg[i] + N*p->stride[i];
        }
    }
    while (n --> 0) {
        const Inst* program = p->inst;
        program->op1(program,program,val,arg);
        for (int i = 0; i < p->args; i++) {
            arg[i] = (char*)arg[i] + p->stride[i];
        }
    }

    free(val);
}
