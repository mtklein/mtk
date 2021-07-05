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
    void (*op)(_Bool one, const struct Inst*, Val* v, void* arg[]);
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

static void op_done(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    (void)v;

    const Program* p;
    memcpy(&p, &inst->ptr, sizeof p);

    for (int i = 0; i < p->args; i++) {
        arg[i] = (char*)arg[i] + (one ? 1*p->stride[i]
                                      : N*p->stride[i]);
    }
}

Program* compile(Builder* b) {
    Program* p = calloc(1, sizeof *p);
    p->inst   = b->inst;
    p->stride = b->stride;
    p->insts  = b->insts;
    p->args   = b->args;

    Inst inst = { .op = op_done };
    memcpy(&inst.ptr, &p, sizeof p);
    push(p->inst,p->insts) = inst;

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

#define next inst[1].op(one,inst+1,v+1,arg)

static void op_ld1_16(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    if (one) { memcpy(v, arg[inst->ptr.ix], 1*2); }
    else     { memcpy(v, arg[inst->ptr.ix], N*2); }
    next;
}
U16 ld1_U16(Builder* b, Ptr ptr) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_ld1_16, .ptr = ptr};
    return (U16){ id };
}
S16 ld1_S16(Builder* b, Ptr ptr) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_ld1_16, .ptr = ptr};
    return (S16){ id };
}
F16 ld1_F16(Builder* b, Ptr ptr) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_ld1_16, .ptr = ptr};
    return (F16){ id };
}

static void op_st1_16(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    if (one) { memcpy(arg[inst->ptr.ix], &v[inst->x], 1*2); }
    else     { memcpy(arg[inst->ptr.ix], &v[inst->x], N*2); }
    next;
}
void st1_U16(Builder* b, Ptr ptr, U16 x) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_st1_16, .ptr = ptr, .x = x.id-id};
}
void st1_S16(Builder* b, Ptr ptr, S16 x) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_st1_16, .ptr = ptr, .x = x.id-id};
}
void st1_F16(Builder* b, Ptr ptr, F16 x) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_st1_16, .ptr = ptr, .x = x.id-id};
}

static void op_st1_32(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    if (one) { memcpy(arg[inst->ptr.ix], &v[inst->x], 1*4); }
    else     { memcpy(arg[inst->ptr.ix], &v[inst->x], N*4); }
    next;
}
void st1_U32(Builder* b, Ptr ptr, U32 x) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_st1_32, .ptr = ptr, .x = x.id-id};
}
void st1_S32(Builder* b, Ptr ptr, S32 x) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_st1_32, .ptr = ptr, .x = x.id-id};
}
void st1_F32(Builder* b, Ptr ptr, F32 x) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_st1_32, .ptr = ptr, .x = x.id-id};
}

static void op_splat_32(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    Val imm = {0};
    imm.u32 += inst->imm.u32;
    *v = imm;
    next;
}
U32 splat_U32(Builder* b, uint32_t imm) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_splat_32, .imm.u32 = imm};
    return (U32){ id };
}
S32 splat_S32(Builder* b, int32_t imm) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_splat_32, .imm.s32 = imm};
    return (S32){ id };
}
F32 splat_F32(Builder* b, float imm) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_splat_32, .imm.f32 = imm};
    return (F32){ id };
}


static void op_add_F16(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    v->f16 = v[inst->x].f16 + v[inst->y].f16;
    next;
}
static void op_sub_F16(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    v->f16 = v[inst->x].f16 - v[inst->y].f16;
    next;
}
static void op_mul_F16(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    v->f16 = v[inst->x].f16 * v[inst->y].f16;
    next;
}
static void op_div_F16(_Bool one, const Inst* inst, Val* v, void* arg[]) {
    v->f16 = v[inst->x].f16 / v[inst->y].f16;
    next;
}
F16 add_F16(Builder* b, F16 x, F16 y) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_add_F16, .x = x.id-id, .y = y.id-id};
    return (F16){ id };
}
F16 sub_F16(Builder* b, F16 x, F16 y) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_sub_F16, .x = x.id-id, .y = y.id-id};
    return (F16){ id };
}
F16 mul_F16(Builder* b, F16 x, F16 y) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_mul_F16, .x = x.id-id, .y = y.id-id};
    return (F16){ id };
}
F16 div_F16(Builder* b, F16 x, F16 y) {
    int id = b->insts;
    push(b->inst,b->insts) = (Inst){.op = op_div_F16, .x = x.id-id, .y = y.id-id};
    return (F16){ id };
}

void run(const Program* p, int n, void* arg[]) {
    Val scratch[16], *v = scratch;
    if (len(scratch) < p->insts) {
        v = malloc((size_t)p->insts * sizeof *v);
    }

    const Inst* start = p->inst;
    void (*op)(_Bool, const Inst*, Val* v, void*[]) = start->op;

    for (int i = 0; i < n/N; i++) { op(0,start,v,arg); }
    for (int i = 0; i < n%N; i++) { op(1,start,v,arg); }

    if (v != scratch) {
        free(v);
    }
}
