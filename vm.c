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
#define LD4_0  0,4, 8,12, 16,20,24,28, 32,36,40,44, 48,52,56,60
#define LD4_1  1,5, 9,13, 17,21,25,29, 33,37,41,45, 49,53,57,61
#define LD4_2  2,6,10,14, 18,22,26,30, 34,38,42,46, 50,54,58,62
#define LD4_3  3,7,11,15, 19,23,27,31, 35,39,43,47, 51,55,59,63

#define CONCAT 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, \
              16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31

#define ST4_1 0,16,32,48
#define ST4   0,16,32,48,  1,17,33,49,  2,18,34,50,  3,19,35,51, \
              4,20,36,52,  5,21,37,53,  6,22,38,54,  7,23,39,55, \
              8,24,40,56,  9,25,41,57, 10,26,42,58, 11,27,43,59, \
             12,28,44,60, 13,29,45,61, 14,30,46,62, 15,31,47,63

#define SPLAT_0 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
#define SPLAT_1 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1
#define SPLAT_2 2,2,2,2, 2,2,2,2, 2,2,2,2, 2,2,2,2
#define SPLAT_3 3,3,3,3, 3,3,3,3, 3,3,3,3, 3,3,3,3


#define cast    __builtin_convertvector
#define shuffle __builtin_shufflevector

typedef int8_t   __attribute__((vector_size(1*N))) s8;
typedef int16_t  __attribute__((vector_size(2*N))) s16;
typedef int32_t  __attribute__((vector_size(4*N))) s32;
typedef uint8_t  __attribute__((vector_size(1*N))) u8;
typedef uint16_t __attribute__((vector_size(2*N))) u16;
typedef uint32_t __attribute__((vector_size(4*N))) u32;
typedef __fp16   __attribute__((vector_size(2*N))) f16;
typedef float    __attribute__((vector_size(4*N))) f32;

typedef union {
    s8  s8;
    s16 s16;
    s32 s32;
    u8  u8;
    u16 u16;
    u32 u32;
    f16 f16;
    f32 f32;
} Val;

typedef struct Inst {
    void (*op)(int n, const struct Inst*, Val* v, void* arg[]);
    int x,y,z,w;
    int ptr;
    int imm;
} Inst;

struct Builder {
    Inst* inst;
    int*  stride;
    int   insts;
    int   args;
    Hash  hash;
};

Builder* builder() {
    Builder* b = calloc(1, sizeof *b);
    return b;
}

// In Builder convention, pointer arguments (from arg()) and value IDs (from no_cse(), cse())
// are all 1-indexed.  This allows testing Inst's ptr,x,y,z,w fields' existence with !=0.

Ptr arg(Builder* b, int stride) {
    push(b->stride,b->args) = stride;
    return (Ptr){b->args};  // 1-indexed
}

#define no_cse(b,...) no_cse_(b, (Inst){__VA_ARGS__})
#define    cse(b,...)    cse_(b, (Inst){__VA_ARGS__})

static int no_cse_(Builder* b, Inst inst) {
    push(b->inst,b->insts) = inst;
    return b->insts;  // 1-indexed
}

typedef struct {
    const Builder* b;
    const Inst*    inst;
    int            id;
    int            unused;
} inst_eq_ctx;

static bool inst_eq(int id, void* vctx) {
    inst_eq_ctx* ctx = vctx;
    if (0 == memcmp(ctx->inst, ctx->b->inst + id-1/*1-indexed*/, sizeof(Inst))) {
        ctx->id = id;
        return true;
    }
    return false;
}

static int cse_(Builder* b, Inst inst) {
    int h = (int)murmur3(0, &inst,sizeof inst);

    for (inst_eq_ctx ctx={.b=b,.inst=&inst}; lookup(&b->hash,h, inst_eq,&ctx);) {
        return ctx.id;
    }

    int id = no_cse_(b,inst);
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

    int ptr    = inst->ptr,
        stride = inst->imm;

    arg[ptr] = (char*)arg[ptr] + (n<N ? 1*stride
                                      : N*stride);
}
op_(inc_arg) {
    op_inc_arg_and_done(n,inst,v,arg);
    next;
}

static bool has_varying_pointer(const Builder* b, const Inst* inst) {
    return inst->ptr
        && b->stride[inst->ptr-1] != 0;
}

static bool is_store(const Builder* b, const Inst* inst) {
    return has_varying_pointer(b,inst)
        && (inst->x || inst->y || inst->z || inst->w);
}

static bool has_side_effect(const Builder* b, const Inst* inst) {
    return is_store(b,inst);
}

struct Program {
    int  vals;
    int  loop;
    Inst inst[];
};

Program* compile(Builder* b) {
    union {
        struct { bool live, loop_dependent; };
        int reordered_id;
    } *meta = calloc((size_t)b->insts, sizeof *meta);

    int live_vals = 0;
    for (int i = b->insts; i --> 0;) {
        const Inst* inst = b->inst+i;
        if (has_side_effect(b,inst)) {
            meta[i].live = true;
        }
        if (meta[i].live) {
            if (inst->x) { meta[inst->x-1].live = true; }
            if (inst->y) { meta[inst->y-1].live = true; }
            if (inst->z) { meta[inst->z-1].live = true; }
            if (inst->w) { meta[inst->w-1].live = true; }
            live_vals++;
        }
    }

    for (int i = 0; i < b->insts; i++) {
        const Inst* inst = b->inst+i;
        meta[i].loop_dependent = has_varying_pointer(b,inst)
                              || (inst->x && meta[inst->x-1].loop_dependent)
                              || (inst->y && meta[inst->y-1].loop_dependent)
                              || (inst->z && meta[inst->z-1].loop_dependent)
                              || (inst->w && meta[inst->w-1].loop_dependent);
    }

    int varying_ptrs = 0;
    for (int ptr = 0; ptr < b->args; ptr++) {
        if (b->stride[ptr] != 0) {
            varying_ptrs++;
        }
    }
    const int insts = live_vals
                    + (varying_ptrs ? varying_ptrs : 1/*op_done*/);

    Program* p = malloc(sizeof *p + sizeof(Inst) * (size_t)insts);
    p->vals = 0;

    for (int loop_dependent = 0; loop_dependent < 2; loop_dependent++) {
        if (loop_dependent) {
            p->loop = p->vals;
        }
        for (int i = 0; i < b->insts; i++) {
            if (meta[i].live && meta[i].loop_dependent == loop_dependent) {
                meta[i].reordered_id = p->vals++;

                // Update inst with reordered argument IDs and translate to Program convention:
                //    - 1-indexed ptr -> 0-indexed ptr
                //    - relative value arguments, writing to *v and reading v[inst->x], etc.
                Inst inst = b->inst[i];
                if (inst.ptr) { inst.ptr--; }
                if (inst.x) { inst.x = meta[inst.x-1].reordered_id - meta[i].reordered_id; }
                if (inst.y) { inst.y = meta[inst.y-1].reordered_id - meta[i].reordered_id; }
                if (inst.z) { inst.z = meta[inst.z-1].reordered_id - meta[i].reordered_id; }
                if (inst.w) { inst.w = meta[inst.w-1].reordered_id - meta[i].reordered_id; }

                p->inst[meta[i].reordered_id] = inst;
            }
        }
    }
    assert(p->vals == live_vals);

    Inst* inst = p->inst + p->vals;
    for (int ptr = 0; ptr < b->args; ptr++) {
        if (b->stride[ptr] != 0) {
            *inst++ = (Inst) {
                .op  = op_inc_arg,
                .ptr = ptr,
                .imm = b->stride[ptr],
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

op_(ld1_8) {
    n<N ? memcpy(v, arg[inst->ptr], 1*1)
        : memcpy(v, arg[inst->ptr], 1*N);
    next;
}
op_(ld1_16) {
    n<N ? memcpy(v, arg[inst->ptr], 2*1)
        : memcpy(v, arg[inst->ptr], 2*N);
    next;
}
op_(ld1_32) {
    n<N ? memcpy(v, arg[inst->ptr], 4*1)
        : memcpy(v, arg[inst->ptr], 4*N);
    next;
}
V8  ld1_8 (Builder* b, Ptr ptr) { return (V8 ){ no_cse(b, op_ld1_8 , .ptr=ptr.ix) }; }
V16 ld1_16(Builder* b, Ptr ptr) { return (V16){ no_cse(b, op_ld1_16, .ptr=ptr.ix) }; }
V32 ld1_32(Builder* b, Ptr ptr) { return (V32){ no_cse(b, op_ld1_32, .ptr=ptr.ix) }; }

op_(st1_8) {
    n<N ? memcpy(arg[inst->ptr], &v[inst->x], 1*1)
        : memcpy(arg[inst->ptr], &v[inst->x], 1*N);
    next;
}
op_(st1_16) {
    n<N ? memcpy(arg[inst->ptr], &v[inst->x], 2*1)
        : memcpy(arg[inst->ptr], &v[inst->x], 2*N);
    next;
}
op_(st1_32) {
    n<N ? memcpy(arg[inst->ptr], &v[inst->x], 4*1)
        : memcpy(arg[inst->ptr], &v[inst->x], 4*N);
    next;
}
void st1_8 (Builder* b, Ptr ptr, V8  x) { no_cse(b, op_st1_8 , .ptr=ptr.ix, .x=x.id); }
void st1_16(Builder* b, Ptr ptr, V16 x) { no_cse(b, op_st1_16, .ptr=ptr.ix, .x=x.id); }
void st1_32(Builder* b, Ptr ptr, V32 x) { no_cse(b, op_st1_32, .ptr=ptr.ix, .x=x.id); }

op_(ld4_8) {
    uint8_t __attribute__((vector_size(4*1*N), aligned(1))) s;
    if (n<N) {
        memcpy(&s, arg[inst->ptr], 4);
        v[0].u8 = shuffle(s,s, SPLAT_0);
        v[1].u8 = shuffle(s,s, SPLAT_1);
        v[2].u8 = shuffle(s,s, SPLAT_2);
        v[3].u8 = shuffle(s,s, SPLAT_3);
    } else {
        memcpy(&s, arg[inst->ptr], sizeof s);
        v[0].u8 = shuffle(s,s, LD4_0);
        v[1].u8 = shuffle(s,s, LD4_1);
        v[2].u8 = shuffle(s,s, LD4_2);
        v[3].u8 = shuffle(s,s, LD4_3);
    }
    inst += 3;
    v    += 3;
    next;
}
struct V8x4 ld4_8(Builder* b, Ptr ptr) {
    int id = no_cse(b, op_ld4_8, .ptr=ptr.ix);
    return (struct V8x4) {
        .r.id = id,
        .g.id = no_cse(b, .x=id),
        .b.id = no_cse(b, .x=id),
        .a.id = no_cse(b, .x=id),
    };
}

op_(st4_8) {
    typedef uint8_t __attribute__((vector_size(4*1  ), aligned(1))) S1;
    typedef uint8_t __attribute__((vector_size(4*1*N), aligned(1))) SN;
    if (n<N) {
        *(S1*)arg[inst->ptr] = shuffle(shuffle(v[inst->x].u8, v[inst->y].u8, CONCAT),
                                       shuffle(v[inst->z].u8, v[inst->w].u8, CONCAT), ST4_1);
    } else {
        *(SN*)arg[inst->ptr] = shuffle(shuffle(v[inst->x].u8, v[inst->y].u8, CONCAT),
                                       shuffle(v[inst->z].u8, v[inst->w].u8, CONCAT), ST4);
    }
    next;
}
void st4_8(Builder* b, Ptr ptr, V8 x, V8 y, V8 z, V8 w) {
    no_cse(b, op_st4_8, .ptr=ptr.ix, .x=x.id, .y=y.id, .z=z.id, .w=w.id);
}

op_(splat_8) {
    Val val = {0};
    val.u8 += (uint8_t)inst->imm;
    *v = val;
    next;
}
op_(splat_16) {
    Val val = {0};
    val.u16 += (uint16_t)inst->imm;
    *v = val;
    next;
}
op_(splat_32) {
    Val val = {0};
    val.u32 += (uint32_t)inst->imm;
    *v = val;
    next;
}
V8  splat_8 (Builder* b, int imm) { return (V8 ){ cse(b, op_splat_8 , .imm=imm) }; }
V16 splat_16(Builder* b, int imm) { return (V16){ cse(b, op_splat_16, .imm=imm) }; }
V32 splat_32(Builder* b, int imm) { return (V32){ cse(b, op_splat_32, .imm=imm) }; }

op_(uniform_8) {
    uint8_t uni;
    memcpy(&uni, (const char*)arg[inst->ptr] + inst->imm, sizeof uni);

    Val val = {0};
    val.u8 += uni;
    *v = val;
    next;
}
op_(uniform_16) {
    uint16_t uni;
    memcpy(&uni, (const char*)arg[inst->ptr] + inst->imm, sizeof uni);

    Val val = {0};
    val.u16 += uni;
    *v = val;
    next;
}
op_(uniform_32) {
    uint32_t uni;
    memcpy(&uni, (const char*)arg[inst->ptr] + inst->imm, sizeof uni);

    Val val = {0};
    val.u32 += uni;
    *v = val;
    next;
}
V8 uniform_8(Builder* b, Ptr ptr, int offset) {
    return (V8){ cse(b, op_uniform_8, .ptr=ptr.ix, .imm=offset) };
}
V16 uniform_16(Builder* b, Ptr ptr, int offset) {
    return (V16){ cse(b, op_uniform_16, .ptr=ptr.ix, .imm=offset) };
}
V32 uniform_32(Builder* b, Ptr ptr, int offset) {
    return (V32){ cse(b, op_uniform_32, .ptr=ptr.ix, .imm=offset) };
}


op_(cast_F16_to_S16) { v->s16 = cast(v[inst->x].f16, s16); next; }
op_(cast_F16_to_U16) { v->u16 = cast(v[inst->x].f16, u16); next; }
op_(cast_S16_to_F16) { v->f16 = cast(v[inst->x].s16, f16); next; }
op_(cast_U16_to_F16) { v->f16 = cast(v[inst->x].u16, f16); next; }

op_(cast_F32_to_S32) { v->s32 = cast(v[inst->x].f32, s32); next; }
op_(cast_F32_to_U32) { v->u32 = cast(v[inst->x].f32, u32); next; }
op_(cast_S32_to_F32) { v->f32 = cast(v[inst->x].s32, f32); next; }
op_(cast_U32_to_F32) { v->f32 = cast(v[inst->x].u32, f32); next; }

V16 cast_F16_to_S16(Builder* b, V16 x) { return (V16){ cse(b, op_cast_F16_to_S16, .x=x.id) }; }
V16 cast_F16_to_U16(Builder* b, V16 x) { return (V16){ cse(b, op_cast_F16_to_U16, .x=x.id) }; }
V16 cast_S16_to_F16(Builder* b, V16 x) { return (V16){ cse(b, op_cast_S16_to_F16, .x=x.id) }; }
V16 cast_U16_to_F16(Builder* b, V16 x) { return (V16){ cse(b, op_cast_U16_to_F16, .x=x.id) }; }

V32 cast_F32_to_S32(Builder* b, V32 x) { return (V32){ cse(b, op_cast_F32_to_S32, .x=x.id) }; }
V32 cast_F32_to_U32(Builder* b, V32 x) { return (V32){ cse(b, op_cast_F32_to_U32, .x=x.id) }; }
V32 cast_S32_to_F32(Builder* b, V32 x) { return (V32){ cse(b, op_cast_S32_to_F32, .x=x.id) }; }
V32 cast_U32_to_F32(Builder* b, V32 x) { return (V32){ cse(b, op_cast_U32_to_F32, .x=x.id) }; }


op_(widen_S8)  { v->s16 = cast(v->s8,  s16); next; }
op_(widen_U8)  { v->u16 = cast(v->u8,  u16); next; }
op_(widen_F16) { v->f32 = cast(v->f16, f32); next; }
op_(widen_S16) { v->s32 = cast(v->s16, s32); next; }
op_(widen_U16) { v->u32 = cast(v->u16, u32); next; }

op_(narrow_F32) { v->f16 = cast(v->f32, f16); next; }
op_(narrow_I32) { v->u16 = cast(v->u32, u16); next; }
op_(narrow_I16) { v->u8  = cast(v->u16,  u8); next; }

V16 widen_S8 (Builder* b, V8  x) { return (V16){ cse(b, op_widen_S8 , .x=x.id) }; }
V16 widen_U8 (Builder* b, V8  x) { return (V16){ cse(b, op_widen_U8 , .x=x.id) }; }
V32 widen_F16(Builder* b, V16 x) { return (V32){ cse(b, op_widen_F16, .x=x.id) }; }
V32 widen_S16(Builder* b, V16 x) { return (V32){ cse(b, op_widen_S16, .x=x.id) }; }
V32 widen_U16(Builder* b, V16 x) { return (V32){ cse(b, op_widen_U16, .x=x.id) }; }

V16 narrow_F32(Builder* b, V32 x) { return (V16){ cse(b, op_narrow_F32, .x=x.id) }; }
V16 narrow_I32(Builder* b, V32 x) { return (V16){ cse(b, op_narrow_I32, .x=x.id) }; }
V8  narrow_I16(Builder* b, V16 x) { return (V8 ){ cse(b, op_narrow_I16, .x=x.id) }; }


op_(add_F16) { v->f16 = cast(cast(v[inst->x].f16,f32) + cast(v[inst->y].f16,f32), f16); next; }
op_(sub_F16) { v->f16 = cast(cast(v[inst->x].f16,f32) - cast(v[inst->y].f16,f32), f16); next; }
op_(mul_F16) { v->f16 = cast(cast(v[inst->x].f16,f32) * cast(v[inst->y].f16,f32), f16); next; }
op_(div_F16) { v->f16 = cast(cast(v[inst->x].f16,f32) / cast(v[inst->y].f16,f32), f16); next; }

V16 add_F16(Builder* b, V16 x, V16 y) { return (V16){ cse(b, op_add_F16, .x=x.id, .y=y.id) }; }
V16 sub_F16(Builder* b, V16 x, V16 y) { return (V16){ cse(b, op_sub_F16, .x=x.id, .y=y.id) }; }
V16 mul_F16(Builder* b, V16 x, V16 y) { return (V16){ cse(b, op_mul_F16, .x=x.id, .y=y.id) }; }
V16 div_F16(Builder* b, V16 x, V16 y) { return (V16){ cse(b, op_div_F16, .x=x.id, .y=y.id) }; }

op_(add_I32) { v->u32 = v[inst->x].u32 + v[inst->y].u32; next; }
op_(sub_I32) { v->u32 = v[inst->x].u32 - v[inst->y].u32; next; }
op_(mul_I32) { v->u32 = v[inst->x].u32 * v[inst->y].u32; next; }

V32 add_I32(Builder* b, V32 x, V32 y) { return (V32){ cse(b, op_add_I32, .x=x.id, .y=y.id) }; }
V32 sub_I32(Builder* b, V32 x, V32 y) { return (V32){ cse(b, op_sub_I32, .x=x.id, .y=y.id) }; }
V32 mul_I32(Builder* b, V32 x, V32 y) { return (V32){ cse(b, op_mul_I32, .x=x.id, .y=y.id) }; }

op_(shl_I32) { v->u32 = v[inst->x].u32 << inst->imm; next; }
op_(shr_S32) { v->s32 = v[inst->x].s32 >> inst->imm; next; }
op_(shr_U32) { v->u32 = v[inst->x].u32 >> inst->imm; next; }

V32 shl_I32(Builder* b, V32 x, int k) { return (V32){ cse(b, op_shl_I32, .x=x.id, .imm=k) }; }
V32 shr_S32(Builder* b, V32 x, int k) { return (V32){ cse(b, op_shr_S32, .x=x.id, .imm=k) }; }
V32 shr_U32(Builder* b, V32 x, int k) { return (V32){ cse(b, op_shr_U32, .x=x.id, .imm=k) }; }

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
