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
    int   vals;
    int   padding;
};

Builder* builder() {
    Builder* b = calloc(1, sizeof *b);
    return b;
}

#define op_(name) static void op_##name(_Bool one, const Inst* inst, Val* v, void* arg[])
#define next inst[1].op(one,inst+1,v+1,arg)

op_(done) {
    (void)one;
    (void)inst;
    (void)v;
    (void)arg;
}
op_(inc_arg_and_done) {
    (void)v;

    int ix     = inst->ptr.ix,
        stride = inst->imm.s32;

    arg[ix] = (char*)arg[ix] + (one ? 1*stride
                                    : N*stride);
}
op_(inc_arg) {
    op_inc_arg_and_done(one,inst,v,arg);
    next;
}

Program* compile(Builder* b) {
    Program* p = calloc(1, sizeof *p);
    p->inst   = b->inst;
    p->vals   = b->insts;

    for (int i = 0; i < b->args; i++) {
        push(p->inst,b->insts) = (Inst) {
            .op      = i == b->args-1 ? op_inc_arg_and_done : op_inc_arg,
            .ptr     = (Ptr){i},
            .imm.s32 = b->stride[i],
        };
    }
    if (b->args == 0) {
        push(p->inst,b->insts) = (Inst){.op=op_done};
    }

    free(b->stride);
    free(b);
    return p;
}

void drop(Program* p) {
    free(p->inst);
    free(p);
}

Ptr arg(Builder* b, int stride) {
    push(b->stride,b->args) = stride;
    return (Ptr){b->args-1};
}

static int push_inst(Builder* b, Inst inst) {
    push(b->inst,b->insts) = inst;
    return b->insts-1;
}

op_(ld1_16) {
    one ? memcpy(v, arg[inst->ptr.ix], 1*2)
        : memcpy(v, arg[inst->ptr.ix], N*2);
    next;
}
U16 ld1_U16(Builder* b, Ptr ptr) {
    return (U16){ push_inst(b, (Inst){.op=op_ld1_16, .ptr=ptr}) };
}
S16 ld1_S16(Builder* b, Ptr ptr) {
    return (S16){ push_inst(b, (Inst){.op=op_ld1_16, .ptr=ptr}) };
}
F16 ld1_F16(Builder* b, Ptr ptr) {
    return (F16){ push_inst(b, (Inst){.op=op_ld1_16, .ptr=ptr}) };
}

op_(ld1_32) {
    one ? memcpy(v, arg[inst->ptr.ix], 1*4)
        : memcpy(v, arg[inst->ptr.ix], N*4);
    next;
}
U32 ld1_U32(Builder* b, Ptr ptr) {
    return (U32){ push_inst(b, (Inst){.op=op_ld1_32, .ptr=ptr}) };
}
S32 ld1_S32(Builder* b, Ptr ptr) {
    return (S32){ push_inst(b, (Inst){.op=op_ld1_32, .ptr=ptr}) };
}
F32 ld1_F32(Builder* b, Ptr ptr) {
    return (F32){ push_inst(b, (Inst){.op=op_ld1_32, .ptr=ptr}) };
}

op_(st1_16) {
    one ? memcpy(arg[inst->ptr.ix], &v[inst->x], 1*2)
        : memcpy(arg[inst->ptr.ix], &v[inst->x], N*2);
    next;
}
void st1_U16(Builder* b, Ptr ptr, U16 x) {
    push_inst(b, (Inst){.op=op_st1_16, .ptr=ptr, .x=x.id-b->insts});
}
void st1_S16(Builder* b, Ptr ptr, S16 x) {
    push_inst(b, (Inst){.op=op_st1_16, .ptr=ptr, .x=x.id-b->insts});
}
void st1_F16(Builder* b, Ptr ptr, F16 x) {
    push_inst(b, (Inst){.op=op_st1_16, .ptr=ptr, .x=x.id-b->insts});
}

op_(st1_32) {
    one ? memcpy(arg[inst->ptr.ix], &v[inst->x], 1*4)
        : memcpy(arg[inst->ptr.ix], &v[inst->x], N*4);
    next;
}
void st1_U32(Builder* b, Ptr ptr, U32 x) {
    push_inst(b, (Inst){.op=op_st1_32, .ptr=ptr, .x=x.id-b->insts});
}
void st1_S32(Builder* b, Ptr ptr, S32 x) {
    push_inst(b, (Inst){.op=op_st1_32, .ptr=ptr, .x=x.id-b->insts});
}
void st1_F32(Builder* b, Ptr ptr, F32 x) {
    push_inst(b, (Inst){.op=op_st1_32, .ptr=ptr, .x=x.id-b->insts});
}

op_(splat_32) {
    Val imm = {0};
    imm.u32 += inst->imm.u32;
    *v = imm;
    next;
}
U32 splat_U32(Builder* b, uint32_t imm) {
    return (U32){ push_inst(b, (Inst){.op = op_splat_32, .imm.u32 = imm}) };
}
S32 splat_S32(Builder* b, int32_t imm) {
    return (S32){ push_inst(b, (Inst){.op = op_splat_32, .imm.s32 = imm}) };
}
F32 splat_F32(Builder* b, float imm) {
    return (F32){ push_inst(b, (Inst){.op = op_splat_32, .imm.f32 = imm}) };
}


op_(add_F16) { v->f16 = v[inst->x].f16 + v[inst->y].f16; next; }
op_(sub_F16) { v->f16 = v[inst->x].f16 - v[inst->y].f16; next; }
op_(mul_F16) { v->f16 = v[inst->x].f16 * v[inst->y].f16; next; }
op_(div_F16) { v->f16 = v[inst->x].f16 / v[inst->y].f16; next; }

F16 add_F16(Builder* b, F16 x, F16 y) {
    return (F16){ push_inst(b, (Inst){.op=op_add_F16, .x=x.id-b->insts, .y=y.id-b->insts}) };
}
F16 sub_F16(Builder* b, F16 x, F16 y) {
    return (F16){ push_inst(b, (Inst){.op=op_sub_F16, .x=x.id-b->insts, .y=y.id-b->insts}) };
}
F16 mul_F16(Builder* b, F16 x, F16 y) {
    return (F16){ push_inst(b, (Inst){.op=op_mul_F16, .x=x.id-b->insts, .y=y.id-b->insts}) };
}
F16 div_F16(Builder* b, F16 x, F16 y) {
    return (F16){ push_inst(b, (Inst){.op=op_div_F16, .x=x.id-b->insts, .y=y.id-b->insts}) };
}

op_(add_S32) { v->s32 = v[inst->x].s32 + v[inst->y].s32; next; }
op_(sub_S32) { v->s32 = v[inst->x].s32 - v[inst->y].s32; next; }
op_(mul_S32) { v->s32 = v[inst->x].s32 * v[inst->y].s32; next; }

S32 add_S32(Builder* b, S32 x, S32 y) {
    return (S32){ push_inst(b, (Inst){.op=op_add_S32, .x=x.id-b->insts, .y=y.id-b->insts}) };
}
S32 sub_S32(Builder* b, S32 x, S32 y) {
    return (S32){ push_inst(b, (Inst){.op=op_sub_S32, .x=x.id-b->insts, .y=y.id-b->insts}) };
}
S32 mul_S32(Builder* b, S32 x, S32 y) {
    return (S32){ push_inst(b, (Inst){.op=op_mul_S32, .x=x.id-b->insts, .y=y.id-b->insts}) };
}

void run(const Program* p, int n, void* arg[]) {
    Val scratch[16], *v = scratch;
    if (len(scratch) < p->vals) {
        v = malloc((size_t)p->vals * sizeof *v);
    }

    const Inst* start = p->inst;
    void (*op)(_Bool, const Inst*, Val* v, void*[]) = start->op;

    for (int i = 0; i < n/N; i++) { op(0,start,v,arg); }
    for (int i = 0; i < n%N; i++) { op(1,start,v,arg); }

    if (v != scratch) {
        free(v);
    }
}
