#include "array.h"
#include "len.h"
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
    void (*op)(const Program*, const struct Inst*, Val* dst, Val val[], void* arg[]);
    int x,y,z,w;
    Ptr ptr;
    Imm imm;
} Inst;

typedef struct {
    void (*opN)(const Program*, const Inst*, Val* dst, Val val[], void* arg[]);
    void (*op1)(const Program*, const Inst*, Val* dst, Val val[], void* arg[]);
    int x,y,z,w;
    Ptr ptr;
    Imm imm;
} BInst;

struct Builder {
    BInst* inst;
    int*   stride;
    int    insts;
    int    args;
};

struct Program {
    int*  stride;
    int   insts;
    int   args;
    Inst  inst[];
};

Builder* builder() {
    Builder* b = calloc(1, sizeof *b);
    return b;
}

static void opN_done(const Program* p, const Inst* inst, Val* dst, Val val[], void* arg[]) {
    (void)inst;
    (void)dst;
    (void)val;
    for (int i = 0; i < p->args; i++) {
        arg[i] = (char*)arg[i] + N*p->stride[i];
    }
}
static void op1_done(const Program* p, const Inst* inst, Val* dst, Val val[], void* arg[]) {
    (void)inst;
    (void)dst;
    (void)val;
    for (int i = 0; i < p->args; i++) {
        arg[i] = (char*)arg[i] + 1*p->stride[i];
    }
}

Program* compile(Builder* b) {
    push(b->inst,b->insts) = (BInst){.opN = opN_done, .op1 = op1_done};

    Program* p = malloc(sizeof *p + 2*(size_t)b->insts * sizeof(Inst));
    p->stride = b->stride;
    p->insts  = b->insts;
    p->args   = b->args;

    for (int i = 0; i < p->insts; i++) {
        p->inst[i] = (Inst) {
            .op  = b->inst[i].opN,
            .x   = b->inst[i].x,
            .y   = b->inst[i].y,
            .z   = b->inst[i].z,
            .w   = b->inst[i].w,
            .ptr = b->inst[i].ptr,
            .imm = b->inst[i].imm,
        };
        p->inst[i+p->insts] = (Inst) {
            .op  = b->inst[i].op1,
            .x   = b->inst[i].x,
            .y   = b->inst[i].y,
            .z   = b->inst[i].z,
            .w   = b->inst[i].w,
            .ptr = b->inst[i].ptr,
            .imm = b->inst[i].imm,
        };
    }

    free(b->inst);
    free(b);
    return p;
}

void drop(Program* p) {
    free(p->stride);
    free(p);
}

Ptr arg(Builder* b, int stride) {
    int ix = b->args;
    push(b->stride,b->args) = stride;
    return (Ptr){ix};
}

#define next inst[1].op(p,inst+1,dst+1,val,arg)

static void op_splat_32(const Program* p, const Inst* inst, Val* dst, Val val[], void* arg[]) {
    Val v = {0};
    v.u32 += inst->imm.u32;
    *dst = v;
    next;
}
U32 splat_U32(Builder* b, uint32_t imm) {
    int id = b->insts;
    push(b->inst,b->insts) = (BInst) {
        .opN     = op_splat_32,
        .op1     = op_splat_32,
        .imm.u32 = imm,
    };
    return (U32){id};
}

static void opN_st1_32(const Program* p, const Inst* inst, Val* dst, Val val[], void* arg[]) {
    memcpy(arg[inst->ptr.ix], &val[inst->x].u32, N*4);
    next;
}
static void op1_st1_32(const Program* p, const Inst* inst, Val* dst, Val val[], void* arg[]) {
    memcpy(arg[inst->ptr.ix], &val[inst->x].u32, 1*4);
    next;
}
void st1_32(Builder* b, Ptr p, U32 v) {
    push(b->inst,b->insts) = (BInst) {
        .opN = opN_st1_32,
        .op1 = op1_st1_32,
        .ptr = p,
        .x   = v.id,
    };
}

void run(const Program* p, int n, void* arg[]) {
    Val scratch[16], *val = scratch;
    if (len(scratch) < p->insts) {
        val = malloc((size_t)p->insts * sizeof *val);
    }

    {
        const Inst* start = p->inst;
        void (*op)(const Program*, const Inst*, Val*, Val[], void*[]) = start->op;
        for (int i = 0; i < n/N; i++) { op(p,start,val,val,arg); }
    }
    {
        const Inst* start = p->inst + p->insts;
        void (*op)(const Program*, const Inst*, Val*, Val[], void*[]) = start->op;
        for (int i = 0; i < n%N; i++) { op(p,start,val,val,arg); }
    }

    if (val != scratch) {
        free(val);
    }
}
