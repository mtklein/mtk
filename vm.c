#include "array.h"
#include "vm.h"
#include <stdlib.h>

typedef union {
    uint8_t  u8;
    uint16_t u16;
    uint32_t u32;
     int8_t  s8;
     int16_t s16;
     int32_t s32;
    _Float16 f16;
    float    f32;
} Val;

typedef struct Inst {
    void (*op)(int id, const struct Inst*, Val val[], void* arg[]);
    int x,y,z,w;
    Ptr ptr;
    Val imm;
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

Program* compile(Builder* b) {
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

static void op_splat(int id, const Inst* inst, Val val[], void* arg[]) {
    (void)arg;
    val[id] = inst->imm;
}
U32 splat_U32(Builder* b, uint32_t imm) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst) {
        .op      = op_splat,
        .imm.u32 = imm,
    };
    return (U32){id};
}

static void op_st1_32(int id, const Inst* inst, Val val[], void* arg[]) {
    (void)id;
    uint32_t* p = arg[inst->ptr.ix];
    *p = val[inst->x].u32;
}
void st1_32(Builder* b, Ptr p, U32 v) {
    push(b->inst,b->insts) = (Inst) {
        .op  = op_st1_32,
        .ptr = p,
        .x   = v.id,
    };
}

void run(const Program* p, int n, void* arg[]) {
    Val* val = calloc((size_t)p->insts, sizeof *val);

    while (n --> 0) {
        for (int i = 0; i < p->insts; i++) {
            const Inst* inst = p->inst+i;
            inst->op(i,inst,val,arg);
        }
        for (int i = 0; i < p->args; i++) {
            arg[i] = (char*)arg[i] + p->stride[i];
        }
    }

    free(val);
}
