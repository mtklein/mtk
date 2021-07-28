#include "array.h"
#include "checksum.h"
#include "hash.h"
#include "len.h"
#include "vm.h"
#include <assert.h>
#include <stdbool.h>
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
    void (*op)(int n, const struct Inst*, Val* v, void* arg[]);
    int x,y,z,w;
    Ptr ptr;
    Imm imm;
} Inst;

struct Builder {
    Inst* inst;
    int*  stride;
    int   insts;
    int   args;
    Hash  hash;
};

typedef struct {
    const Builder* b;
    const Inst*    inst;
} inst_eq_ctx;

static bool inst_eq(int id, void* vctx) {
    const inst_eq_ctx* ctx = vctx;
    return 0 == memcmp(ctx->inst, ctx->b->inst + id-1/*1-indexed, see no_cse()*/, sizeof(Inst));
}

Builder* builder() {
    Builder* b = calloc(1, sizeof *b);
    return b;
}

static int no_cse(Builder* b, Inst inst) {
    push(b->inst,b->insts) = inst;
    return b->insts;  // 1-indexed in Builder, letting !inst.[xyzw] indicate an unused argument.
}

static int cse(Builder* b, Inst inst) {
    int h = (int)murmur3(0, &inst,sizeof inst);

    int id;
    for (inst_eq_ctx ctx={b,&inst}; lookup(&b->hash,h, inst_eq,&ctx, &id);) {
        return id;
    }

    id = no_cse(b,inst);
    insert(&b->hash,h,id);
    return id;
}


#define op_(name) static void op_##name(int n, const Inst* inst, Val* v, void* arg[])
#define next inst[1].op(n,inst+1,v+1,arg)

op_(done) {
    (void)n;
    (void)inst;
    (void)v;
    (void)arg;
}
op_(inc_arg_and_done) {
    (void)v;

    int ix     = inst->ptr.ix,
        stride = inst->imm.s32;

    arg[ix] = (char*)arg[ix] + (n<N ? 1*stride
                                    : N*stride);
}
op_(inc_arg) {
    op_inc_arg_and_done(n,inst,v,arg);
    next;
}

struct Program {
    int  vals;
    int  loop;
    Inst inst[];
};

Program* compile(Builder* b) {
    typedef union {
        struct { bool live, loop_dependent; };
        int new_id;
    } Meta;
    Meta* meta = calloc((size_t)b->insts, sizeof *meta);

    // A value is live if it has a side-effect or if it's used by another live value.
    int live = 0;
    for (int i = b->insts; i --> 0;) {
        const Inst* inst = b->inst+i;

        // Heuristically, instructions that take a value and a varying pointer have side effects.
        if (inst->ptr.ix && b->stride[inst->ptr.ix-1]
                && (inst->x || inst->y || inst->z || inst->w)) {
            meta[i].live = true;
        }

        // Anything a live instruction needs is also live.
        if (meta[i].live) {
            live++;
            if (inst->x) { meta[inst->x-1].live = true; }
            if (inst->y) { meta[inst->y-1].live = true; }
            if (inst->z) { meta[inst->z-1].live = true; }
            if (inst->w) { meta[inst->w-1].live = true; }
        }
    }

    // A value is loop-dependent if it uses a varying pointer or another loop-dependent value.
    for (int i = 0; i < b->insts; i++) {
        const Inst* inst = b->inst+i;
        meta[i].loop_dependent = (inst->ptr.ix && b->stride[inst->ptr.ix-1])
                              || (inst->x && meta[inst->x-1].loop_dependent)
                              || (inst->y && meta[inst->y-1].loop_dependent)
                              || (inst->z && meta[inst->z-1].loop_dependent)
                              || (inst->w && meta[inst->w-1].loop_dependent);
    }

    // Count up how many instructions we'll need, including one for each argument to increment.
    int args_to_inc = 0;
    for (int ix = 0; ix < b->args; ix++) {
        if (b->stride[ix]) {
            args_to_inc++;
        }
    }
    const int insts = live + (args_to_inc ? args_to_inc : 1/*op_done*/);

    Program* p = malloc(sizeof *p + sizeof(Inst) * (size_t)insts);
    p->vals = 0;

    // Reorder instructions so all live loop-independent instructions come first,
    // then live loop-dependent instructions following from p->inst + p->loop.
    //
    // While we're doing that, rewrite ptr and xyzw indices into their final Program conventions:
    //    - 1-indexed pointer indices become 0-indexed;
    //    - 1-indexed xyzw IDs become relative offsets,
    //      so Program instructions write their value to *v, read x from v[inst->x], etc.
    for (int loop_dependent = 0; loop_dependent < 2; loop_dependent++) {
        if (loop_dependent) {
            p->loop = p->vals;
        }
        for (int i = 0; i < b->insts; i++) {
            if (meta[i].live && meta[i].loop_dependent == loop_dependent) {
                meta[i].new_id = p->vals;

                Inst inst = b->inst[i];
                if (inst.ptr.ix) { inst.ptr.ix--; }
                if (inst.x) { inst.x = meta[inst.x-1].new_id; inst.x -= meta[i].new_id; }
                if (inst.y) { inst.y = meta[inst.y-1].new_id; inst.y -= meta[i].new_id; }
                if (inst.z) { inst.z = meta[inst.z-1].new_id; inst.z -= meta[i].new_id; }
                if (inst.w) { inst.w = meta[inst.w-1].new_id; inst.w -= meta[i].new_id; }

                p->inst[p->vals++] = inst;
            }
        }
    }

    // Add a few more non-value-producing instructions to increment each argument and wrap up.
    Inst* inst = p->inst + p->vals;
    for (int ix = 0; ix < b->args; ix++) {
        if (b->stride[ix]) {
            *inst++ = (Inst) {
                .op      = op_inc_arg,
                .ptr.ix  = ix,
                .imm.s32 = b->stride[ix],
            };
        }
    }
    if (inst > p->inst + p->vals) {
        inst[-1].op = op_inc_arg_and_done;
    } else {
        *inst++ = (Inst){.op=op_done};
    }
    assert(inst == p->inst + insts);

    free(meta);
    free(b->inst);
    free(b->stride);
    free(b->hash.table);
    free(b);
    return p;
}

void drop(Program* p) {
    free(p);
}

Ptr arg(Builder* b, int stride) {
    push(b->stride,b->args) = stride;
    return (Ptr){b->args};  // 1-indexed in Builder, letting !inst.ptr.ix indicate an unused ptr.
}

op_(ld1_16) {
    n<N ? memcpy(v, arg[inst->ptr.ix], 1*2)
        : memcpy(v, arg[inst->ptr.ix], N*2);
    next;
}
U16 ld1_U16(Builder* b, Ptr ptr) { return (U16){ no_cse(b, (Inst){.op=op_ld1_16, .ptr=ptr}) }; }
S16 ld1_S16(Builder* b, Ptr ptr) { return (S16){ no_cse(b, (Inst){.op=op_ld1_16, .ptr=ptr}) }; }
F16 ld1_F16(Builder* b, Ptr ptr) { return (F16){ no_cse(b, (Inst){.op=op_ld1_16, .ptr=ptr}) }; }

op_(ld1_32) {
    n<N ? memcpy(v, arg[inst->ptr.ix], 1*4)
        : memcpy(v, arg[inst->ptr.ix], N*4);
    next;
}
U32 ld1_U32(Builder* b, Ptr ptr) { return (U32){ no_cse(b, (Inst){.op=op_ld1_32, .ptr=ptr}) }; }
S32 ld1_S32(Builder* b, Ptr ptr) { return (S32){ no_cse(b, (Inst){.op=op_ld1_32, .ptr=ptr}) }; }
F32 ld1_F32(Builder* b, Ptr ptr) { return (F32){ no_cse(b, (Inst){.op=op_ld1_32, .ptr=ptr}) }; }

op_(st1_16) {
    n<N ? memcpy(arg[inst->ptr.ix], &v[inst->x], 1*2)
        : memcpy(arg[inst->ptr.ix], &v[inst->x], N*2);
    next;
}
void st1_U16(Builder* b, Ptr ptr, U16 x) { no_cse(b, (Inst){.op=op_st1_16, .ptr=ptr, .x=x.id}); }
void st1_S16(Builder* b, Ptr ptr, S16 x) { no_cse(b, (Inst){.op=op_st1_16, .ptr=ptr, .x=x.id}); }
void st1_F16(Builder* b, Ptr ptr, F16 x) { no_cse(b, (Inst){.op=op_st1_16, .ptr=ptr, .x=x.id}); }

op_(st1_32) {
    n<N ? memcpy(arg[inst->ptr.ix], &v[inst->x], 1*4)
        : memcpy(arg[inst->ptr.ix], &v[inst->x], N*4);
    next;
}
void st1_U32(Builder* b, Ptr ptr, U32 x) { no_cse(b, (Inst){.op=op_st1_32, .ptr=ptr, .x=x.id}); }
void st1_S32(Builder* b, Ptr ptr, S32 x) { no_cse(b, (Inst){.op=op_st1_32, .ptr=ptr, .x=x.id}); }
void st1_F32(Builder* b, Ptr ptr, F32 x) { no_cse(b, (Inst){.op=op_st1_32, .ptr=ptr, .x=x.id}); }

op_(splat_32) {
    uint32_t imm = inst->imm.u32;

    Val val = {0};
    val.u32 += imm;
    *v = val;
    next;
}
U32 splat_U32(Builder* b, uint32_t imm) {
    return (U32){ cse(b, (Inst){.op = op_splat_32, .imm.u32 = imm}) };
}
S32 splat_S32(Builder* b, int32_t imm) {
    return (S32){ cse(b, (Inst){.op = op_splat_32, .imm.s32 = imm}) };
}
F32 splat_F32(Builder* b, float imm) {
    return (F32){ cse(b, (Inst){.op = op_splat_32, .imm.f32 = imm}) };
}

op_(uniform_32) {
    uint32_t uni;
    memcpy(&uni, (const char*)arg[inst->ptr.ix] + inst->imm.s32, sizeof uni);

    Val val = {0};
    val.u32 += uni;
    *v = val;
    next;
}
U32 uniform_U32(Builder* b, Ptr ptr, int offset) {
    return (U32){ cse(b, (Inst){.op = op_uniform_32, .ptr = ptr, .imm.s32=offset}) };
}
S32 uniform_S32(Builder* b, Ptr ptr, int offset) {
    return (S32){ cse(b, (Inst){.op = op_uniform_32, .ptr = ptr, .imm.s32=offset}) };
}
F32 uniform_F32(Builder* b, Ptr ptr, int offset) {
    return (F32){ cse(b, (Inst){.op = op_uniform_32, .ptr = ptr, .imm.s32=offset}) };
}

op_(add_F16) { v->f16 = v[inst->x].f16 + v[inst->y].f16; next; }
op_(sub_F16) { v->f16 = v[inst->x].f16 - v[inst->y].f16; next; }
op_(mul_F16) { v->f16 = v[inst->x].f16 * v[inst->y].f16; next; }
op_(div_F16) { v->f16 = v[inst->x].f16 / v[inst->y].f16; next; }

F16 add_F16(Builder* b, F16 x, F16 y) {
    return (F16){ cse(b, (Inst){.op=op_add_F16, .x=x.id, .y=y.id}) };
}
F16 sub_F16(Builder* b, F16 x, F16 y) {
    return (F16){ cse(b, (Inst){.op=op_sub_F16, .x=x.id, .y=y.id}) };
}
F16 mul_F16(Builder* b, F16 x, F16 y) {
    return (F16){ cse(b, (Inst){.op=op_mul_F16, .x=x.id, .y=y.id}) };
}
F16 div_F16(Builder* b, F16 x, F16 y) {
    return (F16){ cse(b, (Inst){.op=op_div_F16, .x=x.id, .y=y.id}) };
}

op_(add_S32) { v->s32 = v[inst->x].s32 + v[inst->y].s32; next; }
op_(sub_S32) { v->s32 = v[inst->x].s32 - v[inst->y].s32; next; }
op_(mul_S32) { v->s32 = v[inst->x].s32 * v[inst->y].s32; next; }

S32 add_S32(Builder* b, S32 x, S32 y) {
    return (S32){ cse(b, (Inst){.op=op_add_S32, .x=x.id, .y=y.id}) };
}
S32 sub_S32(Builder* b, S32 x, S32 y) {
    return (S32){ cse(b, (Inst){.op=op_sub_S32, .x=x.id, .y=y.id}) };
}
S32 mul_S32(Builder* b, S32 x, S32 y) {
    return (S32){ cse(b, (Inst){.op=op_mul_S32, .x=x.id, .y=y.id}) };
}

void run(const Program* p, int n, void* arg[]) {
    Val scratch[16], *val = scratch;
    if (len(scratch) < p->vals) {
        val = malloc((size_t)p->vals * sizeof *val);
    }

    const Inst *start = p->inst,
               *loop  = p->inst + p->loop;

    Val *v = val,
        *l = val + p->loop;

    for (; n; n -= (n<N ? 1 : N)) {
        start->op(n,start,v,arg);
        start = loop;
        v     = l;
    }

    if (val != scratch) {
        free(val);
    }
}
