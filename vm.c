#include "array.h"
#include "assume.h"
#include "checksum.h"
#include "hash.h"
#include "len.h"
#include "vm.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    #include <arm_neon.h>
#endif

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

#define WHATEVER -1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1

#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    #define LO 0,1, 2, 3, 4, 5, 6, 7
    #define HI 8,9,10,11,12,13,14,15
#else
    #define APPLY(M) M(0) M(1) M( 2) M( 3) M( 4) M( 5) M( 6) M( 7) \
                     M(8) M(9) M(10) M(11) M(12) M(13) M(14) M(15)
#endif

#define cast    __builtin_convertvector
#define shuffle __builtin_shufflevector

#define op_(name) \
    __attribute__((flatten)) \
    static void op_##name(int n, const Inst* inst, Val* v, const void* uniforms, void* varying[])
#define next inst[1].op(n,inst+1,v+1,uniforms,varying); return

typedef int8_t   __attribute__((vector_size(1*N))) s8;
typedef int16_t  __attribute__((vector_size(2*N))) s16;
typedef int32_t  __attribute__((vector_size(4*N))) s32;
typedef uint8_t  __attribute__((vector_size(1*N))) u8;
typedef uint16_t __attribute__((vector_size(2*N))) u16;
typedef uint32_t __attribute__((vector_size(4*N))) u32;
#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    typedef _Float16 __attribute__((vector_size(2*N))) f16;
#else
    typedef __fp16   __attribute__((vector_size(2*N))) f16;
#endif
typedef float    __attribute__((vector_size(4*N))) f32;

typedef union {
    s8 s8; s16 s16; s32 s32;
    u8 u8; u16 u16; u32 u32;
           f16 f16; f32 f32;
} Val;

typedef struct Inst {
    void (*op         )(int n, const struct Inst*, Val* v, const void* uniforms, void* varying[]);
    void (*op_and_done)(int n, const struct Inst*, Val* v, const void* uniforms, void* varying[]);
    int x,y,z,w;
    int imm;
    enum { MATH, SPLAT, UNIFORM, LOAD, STORE } kind;
} Inst;

struct Builder {
    Inst* inst;
    int   insts;
    int   varying;
    Hash  hash;
};

Builder* builder() {
    Builder* b = calloc(1, sizeof *b);
    return b;
}

op_(done) {
    (void)n;
    (void)inst;
    (void)v;
    (void)uniforms;
    (void)varying;
}

typedef struct {
    const Builder* b;
    const Inst*    inst;
    int            id;
    int            unused;
} inst_eq_ctx;

static bool inst_eq(int id, void* vctx) {
    inst_eq_ctx* ctx = vctx;
    return 0 == memcmp(ctx->inst, ctx->b->inst + id-1/*1-indexed*/, sizeof(Inst))
        && (ctx->id = id);
}

static int inst_(int size, Builder* b, Inst inst) {
    int hash = (int)murmur3(0, &inst,sizeof inst);
    for (inst_eq_ctx ctx={.b=b,.inst=&inst}; lookup(&b->hash,hash, inst_eq,&ctx);) {
        return ctx.id;
    }

    while (inst.kind == MATH) {
        Inst constant_prop[6], *p=constant_prop;
        if (inst.x && (*p++ = b->inst[inst.x-1]).kind != SPLAT) { break; }
        if (inst.y && (*p++ = b->inst[inst.y-1]).kind != SPLAT) { break; }
        if (inst.z && (*p++ = b->inst[inst.z-1]).kind != SPLAT) { break; }
        if (inst.w && (*p++ = b->inst[inst.w-1]).kind != SPLAT) { break; }

        int id = (int)(p - constant_prop);
        if (inst.x) { inst.x = 0-id; }
        if (inst.y) { inst.y = 1-id; }
        if (inst.z) { inst.z = 2-id; }
        if (inst.w) { inst.w = 3-id; }
        *p++ = inst;
        *p++ = (Inst){.op=op_done};

        Val v[5];
        constant_prop->op(1,constant_prop,v,NULL,NULL);

        switch (size) {
            case 8 : return splat_8 (b, v[id].s8 [0]).id;
            case 16: return splat_16(b, v[id].s16[0]).id;
            case 32: return splat_32(b, v[id].s32[0]).id;
        }
        assume(false);
    }

    // In Builder convention, IDs are 1-indexed so we can test x,y,z,w arg existence with !=0.
    push(b->inst,b->insts) = inst;
    int id = b->insts;
    if (inst.kind <= UNIFORM) {
        insert(&b->hash,hash,id);
    }
    return id;
}
#define inst(size,b,...) (V##size){inst_(size, b, (Inst){__VA_ARGS__})}


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
        if (inst->kind == STORE) {
            meta[i].live = true;
        }
        if (meta[i].live) {
            live_vals++;
            if (inst->x) { meta[inst->x-1].live = true; }
            if (inst->y) { meta[inst->y-1].live = true; }
            if (inst->z) { meta[inst->z-1].live = true; }
            if (inst->w) { meta[inst->w-1].live = true; }
        }
    }

    for (int i = 0; i < b->insts; i++) {
        const Inst* inst = b->inst+i;
        meta[i].loop_dependent = inst->kind >= LOAD
                              || (inst->x && meta[inst->x-1].loop_dependent)
                              || (inst->y && meta[inst->y-1].loop_dependent)
                              || (inst->z && meta[inst->z-1].loop_dependent)
                              || (inst->w && meta[inst->w-1].loop_dependent);
    }

    Program* p = malloc(sizeof *p + sizeof(Inst) * (size_t)(live_vals ? live_vals : 1/*op_done*/));
    p->vals = 0;

    for (int loop_dependent = 0; loop_dependent < 2; loop_dependent++) {
        if (loop_dependent) {
            p->loop = p->vals;
        }
        for (int i = 0; i < b->insts; i++) {
            if (meta[i].live && meta[i].loop_dependent == loop_dependent) {
                Inst* inst = p->inst + p->vals;
                *inst = b->inst[i];

                // Program uses relative value args, writing to *v, reading from v[inst->x], etc.
                if (inst->x) { inst->x = meta[inst->x-1].reordered_id - p->vals; }
                if (inst->y) { inst->y = meta[inst->y-1].reordered_id - p->vals; }
                if (inst->z) { inst->z = meta[inst->z-1].reordered_id - p->vals; }
                if (inst->w) { inst->w = meta[inst->w-1].reordered_id - p->vals; }

                meta[i].reordered_id = p->vals++;
            }
        }
    }
    assume(p->vals == live_vals);

    if (p->vals) {
        assume(p->inst[p->vals-1].op_and_done);
        p->inst[p->vals-1].op = p->inst[p->vals-1].op_and_done;
    } else {
        p->inst[0] = (Inst){.op=op_done};
    }

    free(meta);
    free(b->inst);
    free(b->hash.table);
    free(b);
    return p;
}

void drop(Program* p) {
    free(p);
}

op_(ld1_8) {
    void** var = varying + inst->imm;
    uint8_t* p = *var;
    if (n<N) { memcpy(v, p,   sizeof *p); *var = p+1; }
    else     { memcpy(v, p, N*sizeof *p); *var = p+N; }
    next;
}
op_(ld1_16) {
    void** var = varying + inst->imm;
    uint16_t* p = *var;
    if (n<N) { memcpy(v, p,   sizeof *p); *var = p+1; }
    else     { memcpy(v, p, N*sizeof *p); *var = p+N; }
    next;
}
op_(ld1_32) {
    void** var = varying + inst->imm;
    uint32_t* p = *var;
    if (n<N) { memcpy(v, p,   sizeof *p); *var = p+1; }
    else     { memcpy(v, p, N*sizeof *p); *var = p+N; }
    next;
}

V8  ld1_8 (Builder* b) { return inst(8 , b, op_ld1_8 , .imm=b->varying++, .kind=LOAD); }
V16 ld1_16(Builder* b) { return inst(16, b, op_ld1_16, .imm=b->varying++, .kind=LOAD); }
V32 ld1_32(Builder* b) { return inst(32, b, op_ld1_32, .imm=b->varying++, .kind=LOAD); }

op_(st1_8_and_done) {
    (void)uniforms;
    void** var = varying + inst->imm;
    uint8_t* p = *var;
    if (n<N) { memcpy(p, &v[inst->x],   sizeof *p); *var = p+1; }
    else     { memcpy(p, &v[inst->x], N*sizeof *p); *var = p+N; }
}
op_(st1_16_and_done) {
    (void)uniforms;
    void** var = varying + inst->imm;
    uint16_t* p = *var;
    if (n<N) { memcpy(p, &v[inst->x],   sizeof *p); *var = p+1; }
    else     { memcpy(p, &v[inst->x], N*sizeof *p); *var = p+N; }
}
op_(st1_32_and_done) {
    (void)uniforms;
    void** var = varying + inst->imm;
    uint32_t* p = *var;
    if (n<N) { memcpy(p, &v[inst->x],   sizeof *p); *var = p+1; }
    else     { memcpy(p, &v[inst->x], N*sizeof *p); *var = p+N; }
}
op_(st1_8 ) { op_st1_8_and_done (n,inst,v,uniforms,varying); next; }
op_(st1_16) { op_st1_16_and_done(n,inst,v,uniforms,varying); next; }
op_(st1_32) { op_st1_32_and_done(n,inst,v,uniforms,varying); next; }

void st1_8 (Builder* b, V8  x) {
    inst(8 , b, op_st1_8 , op_st1_8_and_done , .x=x.id, .imm=b->varying++, .kind=STORE);
}
void st1_16(Builder* b, V16 x) {
    inst(16, b, op_st1_16, op_st1_16_and_done, .x=x.id, .imm=b->varying++, .kind=STORE);
}
void st1_32(Builder* b, V32 x) {
    inst(32, b, op_st1_32, op_st1_32_and_done, .x=x.id, .imm=b->varying++, .kind=STORE);
}

op_(ld4_8) {
    void** var = varying + inst->imm;
    uint8_t* p = *var;
    uint8_t __attribute__((vector_size(4*N<<0), aligned(1))) s;
    if (n<N) {
        memcpy(&s, p, 4);
        v[0].u8 = shuffle(s,s, 0,WHATEVER);
        v[1].u8 = shuffle(s,s, 1,WHATEVER);
        v[2].u8 = shuffle(s,s, 2,WHATEVER);
        v[3].u8 = shuffle(s,s, 3,WHATEVER);
        *var = p+4;
    } else {
        memcpy(&s, p, sizeof s);
        v[0].u8 = shuffle(s,s, LD4_0);
        v[1].u8 = shuffle(s,s, LD4_1);
        v[2].u8 = shuffle(s,s, LD4_2);
        v[3].u8 = shuffle(s,s, LD4_3);
        *var = p+4*N;
    }
    inst += 3;
    v    += 3;
    next;
}
struct V8x4 ld4_8(Builder* b) {
    V8 r = inst(8, b, op_ld4_8, .imm=b->varying++, .kind=LOAD);
    return (struct V8x4) {
        .r = r,
        .g = inst(8, b, .x=r.id, .kind=LOAD),
        .b = inst(8, b, .x=r.id, .kind=LOAD),
        .a = inst(8, b, .x=r.id, .kind=LOAD),
    };
}

op_(st4_8_and_done) {
    (void)uniforms;
    typedef uint8_t __attribute__((vector_size(4*1<<0), aligned(1))) S1;
    typedef uint8_t __attribute__((vector_size(4*N<<0), aligned(1))) SN;

    void** var = varying + inst->imm;
    uint8_t* p = *var;
    if (n<N) {
        *(S1*)p = shuffle(shuffle(v[inst->x].u8, v[inst->y].u8, CONCAT),
                          shuffle(v[inst->z].u8, v[inst->w].u8, CONCAT), ST4_1);
        *var = p+4;
    } else {
        *(SN*)p = shuffle(shuffle(v[inst->x].u8, v[inst->y].u8, CONCAT),
                          shuffle(v[inst->z].u8, v[inst->w].u8, CONCAT), ST4);
        *var = p+4*N;
    }
}
op_(st4_8) { op_st4_8_and_done(n,inst,v,uniforms,varying); next; }

void st4_8(Builder* b, V8 x, V8 y, V8 z, V8 w) {
    inst(8, b, op_st4_8, op_st4_8_and_done,
         .x=x.id, .y=y.id, .z=z.id, .w=w.id, .imm=b->varying++, .kind=STORE);
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
V8  splat_8 (Builder* b, int imm) { return inst(8 , b, op_splat_8 , .imm=imm, .kind=SPLAT); }
V16 splat_16(Builder* b, int imm) { return inst(16, b, op_splat_16, .imm=imm, .kind=SPLAT); }
V32 splat_32(Builder* b, int imm) { return inst(32, b, op_splat_32, .imm=imm, .kind=SPLAT); }

op_(uniform_8) {
    uint8_t uni;
    memcpy(&uni, (const char*)uniforms + inst->imm, sizeof uni);

    Val val = {0};
    val.u8 += uni;
    *v = val;
    next;
}
op_(uniform_16) {
    uint16_t uni;
    memcpy(&uni, (const char*)uniforms + inst->imm, sizeof uni);

    Val val = {0};
    val.u16 += uni;
    *v = val;
    next;
}
op_(uniform_32) {
    uint32_t uni;
    memcpy(&uni, (const char*)uniforms + inst->imm, sizeof uni);

    Val val = {0};
    val.u32 += uni;
    *v = val;
    next;
}
V8  uniform_8 (Builder* b, int offset) {
    return inst(8 , b, op_uniform_8 , .imm=offset, .kind=UNIFORM);
}
V16 uniform_16(Builder* b, int offset) {
    return inst(16, b, op_uniform_16, .imm=offset, .kind=UNIFORM);
}
V32 uniform_32(Builder* b, int offset) {
    return inst(32, b, op_uniform_32, .imm=offset, .kind=UNIFORM);
}


op_(cast_F16_to_S16) { v->s16 = cast(v[inst->x].f16, s16); next; }
op_(cast_F16_to_U16) { v->u16 = cast(v[inst->x].f16, u16); next; }
op_(cast_S16_to_F16) { v->f16 = cast(v[inst->x].s16, f16); next; }
op_(cast_U16_to_F16) { v->f16 = cast(v[inst->x].u16, f16); next; }
op_(cast_F32_to_S32) { v->s32 = cast(v[inst->x].f32, s32); next; }
op_(cast_F32_to_U32) { v->u32 = cast(v[inst->x].f32, u32); next; }
op_(cast_S32_to_F32) { v->f32 = cast(v[inst->x].s32, f32); next; }
op_(cast_U32_to_F32) { v->f32 = cast(v[inst->x].u32, f32); next; }

V16 cast_F16_to_S16(Builder* b, V16 x) { return inst(16, b, op_cast_F16_to_S16, .x=x.id); }
V16 cast_F16_to_U16(Builder* b, V16 x) { return inst(16, b, op_cast_F16_to_U16, .x=x.id); }
V16 cast_S16_to_F16(Builder* b, V16 x) { return inst(16, b, op_cast_S16_to_F16, .x=x.id); }
V16 cast_U16_to_F16(Builder* b, V16 x) { return inst(16, b, op_cast_U16_to_F16, .x=x.id); }
V32 cast_F32_to_S32(Builder* b, V32 x) { return inst(32, b, op_cast_F32_to_S32, .x=x.id); }
V32 cast_F32_to_U32(Builder* b, V32 x) { return inst(32, b, op_cast_F32_to_U32, .x=x.id); }
V32 cast_S32_to_F32(Builder* b, V32 x) { return inst(32, b, op_cast_S32_to_F32, .x=x.id); }
V32 cast_U32_to_F32(Builder* b, V32 x) { return inst(32, b, op_cast_U32_to_F32, .x=x.id); }


op_( widen_S8 ) { v->s16 = cast(v[inst->x].s8 , s16); next; }
op_( widen_U8 ) { v->u16 = cast(v[inst->x].u8 , u16); next; }
op_( widen_F16) { v->f32 = cast(v[inst->x].f16, f32); next; }
op_( widen_S16) { v->s32 = cast(v[inst->x].s16, s32); next; }
op_( widen_U16) { v->u32 = cast(v[inst->x].u16, u32); next; }
op_(narrow_F32) { v->f16 = cast(v[inst->x].f32, f16); next; }
op_(narrow_I32) { v->u16 = cast(v[inst->x].u32, u16); next; }
op_(narrow_I16) { v->u8  = cast(v[inst->x].u16, u8 ); next; }

V16  widen_S8 (Builder* b, V8  x) { return inst(16, b,  op_widen_S8 , .x=x.id); }
V16  widen_U8 (Builder* b, V8  x) { return inst(16, b,  op_widen_U8 , .x=x.id); }
V32  widen_F16(Builder* b, V16 x) { return inst(32, b,  op_widen_F16, .x=x.id); }
V32  widen_S16(Builder* b, V16 x) { return inst(32, b,  op_widen_S16, .x=x.id); }
V32  widen_U16(Builder* b, V16 x) { return inst(32, b,  op_widen_U16, .x=x.id); }
V16 narrow_F32(Builder* b, V32 x) { return inst(16, b, op_narrow_F32, .x=x.id); }
V16 narrow_I32(Builder* b, V32 x) { return inst(16, b, op_narrow_I32, .x=x.id); }
V8  narrow_I16(Builder* b, V16 x) { return inst(8 , b, op_narrow_I16, .x=x.id); }


op_(add_F16) { v->f16 = cast(cast(v[inst->x].f16,f32) + cast(v[inst->y].f16,f32), f16); next; }
op_(sub_F16) { v->f16 = cast(cast(v[inst->x].f16,f32) - cast(v[inst->y].f16,f32), f16); next; }
op_(mul_F16) { v->f16 = cast(cast(v[inst->x].f16,f32) * cast(v[inst->y].f16,f32), f16); next; }
op_(div_F16) { v->f16 = cast(cast(v[inst->x].f16,f32) / cast(v[inst->y].f16,f32), f16); next; }

#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    op_(mla_F16) { v->f16 =  v[inst->x].f16 * v[inst->y].f16 + v[inst->z].f16; next; }
    op_(mls_F16) { v->f16 =  v[inst->x].f16 * v[inst->y].f16 - v[inst->z].f16; next; }
    op_(nma_F16) { v->f16 = -v[inst->x].f16 * v[inst->y].f16 + v[inst->z].f16; next; }
#else
    op_(mla_F16) {
        v->f16 = cast( cast(v[inst->x].f16,f32)
                    *  cast(v[inst->y].f16,f32)
                    +  cast(v[inst->z].f16,f32), f16);
        next;
    }
    op_(mls_F16) {
        v->f16 = cast( cast(v[inst->x].f16,f32)
                    *  cast(v[inst->y].f16,f32)
                    -  cast(v[inst->z].f16,f32), f16);
        next;
    }
    op_(nma_F16) {
        v->f16 = cast(-cast(v[inst->x].f16,f32)
                    *  cast(v[inst->y].f16,f32)
                    +  cast(v[inst->z].f16,f32), f16);
        next;
    }
#endif

static bool equiv(float x, float y) {
    return (x <= y && y <= x)
        || (x != x && y != y);
}
static bool is_splat_F16(Inst inst, float imm) {
    union {
        int bits;
        __fp16 f;
    } pun = {inst.imm};
    return inst.kind == SPLAT
        && equiv((float)pun.f, imm);
}

V16 add_F16(Builder* b, V16 x, V16 y) {
    if (is_splat_F16(b->inst[x.id-1], 0.0f)) { return y; }
    if (is_splat_F16(b->inst[y.id-1], 0.0f)) { return x; }
    for (Inst mul = b->inst[x.id-1]; mul.op == op_mul_F16; ) {
        return inst(16, b, op_mla_F16, .x=mul.x, .y=mul.y, .z=y.id);
    }
    for (Inst mul = b->inst[y.id-1]; mul.op == op_mul_F16; ) {
        return inst(16, b, op_mla_F16, .x=mul.x, .y=mul.y, .z=x.id);
    }
    return inst(16, b, op_add_F16, .x=x.id, .y=y.id);
}
V16 sub_F16(Builder* b, V16 x, V16 y) {
    if (is_splat_F16(b->inst[y.id-1], 0.0f)) { return x; }
    for (Inst mul = b->inst[x.id-1]; mul.op == op_mul_F16; ) {
        return inst(16, b, op_mls_F16, .x=mul.x, .y=mul.y, .z=y.id);
    }
    for (Inst mul = b->inst[y.id-1]; mul.op == op_mul_F16; ) {
        return inst(16, b, op_nma_F16, .x=mul.x, .y=mul.y, .z=x.id);
    }
    return inst(16, b, op_sub_F16, .x=x.id, .y=y.id);
}
V16 mul_F16(Builder* b, V16 x, V16 y) {
    if (is_splat_F16(b->inst[x.id-1], 1.0f)) { return y; }
    if (is_splat_F16(b->inst[y.id-1], 1.0f)) { return x; }
    return inst(16, b, op_mul_F16, .x=x.id, .y=y.id);
}
V16 div_F16(Builder* b, V16 x, V16 y) {
    if (is_splat_F16(b->inst[y.id-1], 1.0f)) { return x; }
    return inst(16, b, op_div_F16, .x=x.id, .y=y.id);
}

op_(sqrt_F16) {
#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
    f16 x = v[inst->x].f16;
    v->f16 = (f16)shuffle(vsqrtq_f16((float16x8_t)shuffle(x,x, LO)),
                          vsqrtq_f16((float16x8_t)shuffle(x,x, HI)), LO,HI);
#else
    f32 x = cast(v[inst->x].f16, f32);
    #define M(i) (__fp16)sqrtf(x[i]),
    v->f16 = (f16){ APPLY(M) };
    #undef M
#endif
    next;
}
V16 sqrt_F16(Builder* b, V16 x) { return inst(16, b, op_sqrt_F16, .x=x.id); }


op_(add_I32) { v->u32 = v[inst->x].u32 + v[inst->y].u32; next; }
op_(sub_I32) { v->u32 = v[inst->x].u32 - v[inst->y].u32; next; }
op_(mul_I32) { v->u32 = v[inst->x].u32 * v[inst->y].u32; next; }
op_(shl_I32) { v->u32 = v[inst->x].u32 << inst->imm; next; }
op_(shr_S32) { v->s32 = v[inst->x].s32 >> inst->imm; next; }
op_(shr_U32) { v->u32 = v[inst->x].u32 >> inst->imm; next; }

V32 add_I32(Builder* b, V32 x, V32 y) { return inst(32, b, op_add_I32, .x=x.id, .y=y.id); }
V32 sub_I32(Builder* b, V32 x, V32 y) { return inst(32, b, op_sub_I32, .x=x.id, .y=y.id); }
V32 mul_I32(Builder* b, V32 x, V32 y) { return inst(32, b, op_mul_I32, .x=x.id, .y=y.id); }
V32 shl_I32(Builder* b, V32 x, int k) { return inst(32, b, op_shl_I32, .x=x.id, .imm=k); }
V32 shr_S32(Builder* b, V32 x, int k) { return inst(32, b, op_shr_S32, .x=x.id, .imm=k); }
V32 shr_U32(Builder* b, V32 x, int k) { return inst(32, b, op_shr_U32, .x=x.id, .imm=k); }


void run(const Program* p, int n, const void* uniforms, void* varying[]) {
    Val scratch[16], *val = scratch;
    if (len(scratch) < p->vals) {
        val = malloc((size_t)p->vals * sizeof *val);
    }

    const Inst *start = p->inst,
               *loop  = p->inst + p->loop;

    Val *v = val,
        *l = val + p->loop;

    for (; n; n -= (n<N ? 1 : N)) {
        start->op(n,start,v,uniforms,varying);
        start = loop;
        v     = l;
    }

    if (val != scratch) {
        free(val);
    }
}
