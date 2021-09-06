#include "len.h"
#include "test.h"
#include "vm.h"
#include <math.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wfloat-equal"

static int fp16(float x) {
    union {
        __fp16  f;
        int16_t bits;
    } pun = {(__fp16)x};
    return pun.bits;
}

static void test_nothing() {
    Program* p = compile(builder());
    run(p, 42, NULL, NULL);
    drop(p);
}

static void test_memset32() {
    Program* p;
    {
        Builder* b = builder();
        st1_32(b, splat_32(b, (int)0xffaaccee));

        p = compile(b);
    }

    uint32_t buf[63] = {0};
    run(p, len(buf), NULL, (void*[]){buf});

    for (int i = 0; i < len(buf); i++) {
        expect_eq(buf[i], 0xffaaccee);
    }

    drop(p);
}

static void test_memset32_uniform() {
    Program* p;
    {
        Builder* b = builder();
        st1_32(b, uniform_32(b,3));

        p = compile(b);
    }

    uint8_t uniforms[] = { 0,1,2,0xee,0xcc,0xaa,0xff,4 };
    uint32_t buf[63] = {0};
    run(p, len(buf), uniforms, (void*[]){buf});

    for (int i = 0; i < len(buf); i++) {
        expect_eq(buf[i], 0xffaaccee);
    }

    drop(p);
}

static void test_F16() {
    Program* p;
    {
        Builder* b = builder();

        V16 x = ld1_16(b),
            y = ld1_16(b);

        V16 v;
        v = add_F16(b, x,y);
        v = mul_F16(b, v,splat_16(b, fp16(2.000f)));
        v = sub_F16(b, v,splat_16(b, fp16(0.125f)));
        v = div_F16(b, v,splat_16(b, fp16(2.000f)));
        st1_16(b,v);

        p = compile(b);
    }

    __fp16 x[63],
           y[len(x)];

    for (int i = 0; i < len(x); i++) {
        x[i] = 0.125f;
        y[i] = 0.250f;
    }

    run(p,len(x), NULL, (void*[]){x,y,x});
    for (int i = 0; i < len(x); i++) {
        expect_eq((float)x[i], 0.3125f);
        expect_eq((float)y[i], 0.2500f);
    }
    drop(p);
}

static void test_cse() {
    Program* p;
    {
        Builder* b = builder();

        V32 x =   ld1_32(b),
            y = splat_32(b, 3),
            z =  add_I32(b, x,y),
            w =  add_I32(b, x,y);
        st1_32(b, mul_I32(b, z,w));

        expect_eq(z.id, w.id);

        p = compile(b);
    }

    int32_t xs[63];
    for (int i = 0; i < len(xs); i++) {
        xs[i] = i;
    }

    run(p,len(xs),NULL,(void*[]){xs,xs});

    for (int i = 0; i < len(xs); i++) {
        expect_eq(xs[i], (i+3)*(i+3));
    }
    drop(p);
}

static void test_dce() {
    Program* p;
    {
        Builder* b = builder();

        V32 x = ld1_32(b);
        x = splat_32(b, 0x42);
        st1_32(b,x);

        p = compile(b);
    }

    int32_t xs[63];
    run(p,len(xs),NULL,(void*[]){NULL,xs});
    for (int i = 0; i < len(xs); i++) {
        expect_eq(xs[i], 0x42);
    }
    drop(p);
}

static void test_structs() {
    Program* p;
    {
        Builder* b = builder();

        struct V8x4 px = ld4_8(b);
        st4_8(b, px.b, px.g, px.r, px.a);

        p = compile(b);
    }

    uint32_t px[63];
    for (int i = 0; i < len(px); i++) {
        px[i] = (uint32_t)((i+0) <<  0)
              | (uint32_t)((i+1) <<  8)
              | (uint32_t)((i+2) << 16)
              | (uint32_t)((i+3) << 24);
    }
    run(p,len(px), NULL, (void*[]){px,px});
    for (int i = 0; i < len(px); i++) {
        expect_eq(px[i], (uint32_t)((i+2) <<  0)
                       | (uint32_t)((i+1) <<  8)
                       | (uint32_t)((i+0) << 16)
                       | (uint32_t)((i+3) << 24));
    }
    drop(p);
}

static void test_constant_prop() {
    Program* p;
    {
        Builder* b = builder();

        V32 x = splat_32(b, 42),
            y = splat_32(b, 47),
            z =  add_I32(b,x,y),
            w = splat_32(b, 89);
        st1_32(b, z);

        expect_eq(z.id, w.id);

        p = compile(b);
    }

    int32_t xs[63];
    run(p,len(xs), NULL, (void*[]){xs});
    for (int i = 0; i < len(xs); i++) {
        expect_eq(xs[i], 42+47);
    }
    drop(p);
}

static void test_peephole() {
    Builder* b = builder();

    V16 x     = uniform_16(b, 0),
        one   =   splat_16(b, fp16( 1.0f)),
        pzero =   splat_16(b, fp16( 0.0f)),
        nzero =   splat_16(b, fp16(-0.0f));


    expect_eq(x.id, add_F16(b, x,pzero).id);
    expect_eq(x.id, add_F16(b, x,nzero).id);
    expect_eq(x.id, add_F16(b, pzero,x).id);
    expect_eq(x.id, add_F16(b, nzero,x).id);

    expect_eq(x.id, sub_F16(b, x,pzero).id);
    expect_eq(x.id, sub_F16(b, x,nzero).id);
    expect_ne(x.id, sub_F16(b, pzero,x).id);
    expect_ne(x.id, sub_F16(b, nzero,x).id);

    expect_eq(x.id, mul_F16(b, x,one).id);
    expect_eq(x.id, mul_F16(b, one,x).id);

    expect_eq(x.id, div_F16(b, x,one).id);
    expect_ne(x.id, div_F16(b, one,x).id);

    drop(compile(b));
}

static void test_mul_add_fusion() {
    Program* p;
    {
        Builder* b = builder();

        V16 x   =   ld1_16(b),
            one = splat_16(b, fp16(1.0f)),
            two = splat_16(b, fp16(2.0f));

        st1_16(b, add_F16(b, one, mul_F16(b, x,two)));
        st1_16(b, add_F16(b, mul_F16(b, x,two), one));

        p = compile(b);
    }

    __fp16 left[63],
          right[len(left)];
    for (int i = 0; i < len(left); i++) {
        left[i] = (__fp16)(float)i;
    }
    run(p,len(left), NULL, (void*[]){left,left,right});
    for (int i = 0; i < len(left); i++) {
        expect_eq((float)left [i], (float)i * 2 + 1);
        expect_eq((float)right[i], (float)i * 2 + 1);
    }
    drop(p);
}

static void test_mul_sub_fusion() {
    Program* p;
    {
        Builder* b = builder();

        V16 x   =   ld1_16(b),
            one = splat_16(b, fp16(1.0f)),
            two = splat_16(b, fp16(2.0f));

        st1_16(b, sub_F16(b, one, mul_F16(b, x,two)));
        st1_16(b, sub_F16(b, mul_F16(b, x,two), one));

        p = compile(b);
    }

    __fp16 left[63],
          right[len(left)];
    for (int i = 0; i < len(left); i++) {
        left[i] = (__fp16)(float)i;
    }
    run(p,len(left), NULL, (void*[]){left,left,right});
    for (int i = 0; i < len(left); i++) {
        expect_eq((float)left [i], 1 - (float)i * 2);
        expect_eq((float)right[i], (float)i * 2 - 1);
    }
    drop(p);
}

static void test_sqrt() {
    {
        Builder* b = builder();
        V16 x,y;

        x = splat_16(b, fp16(0.0f));
        y = sqrt_F16(b, x);
        expect_eq(x.id, y.id);

        x = splat_16(b, fp16(1.0f));
        y = sqrt_F16(b, x);
        expect_eq(x.id, y.id);

        drop(compile(b));
    }

    Program* p;
    {
        Builder* b = builder();

        V16 x = ld1_16(b);
        st1_16(b, sqrt_F16(b,x));

        p = compile(b);
    }

    __fp16 xs[63];
    for (int i = 0; i < len(xs); i++) {
        xs[i] = (__fp16)(float)i;
    }
    run(p,len(xs), NULL, (void*[]){xs,xs});

    for (int i = 0; i < len(xs); i++) {
        float expected = sqrtf((float)i);
        expect_in((float)xs[i], expected * 0.999f
                              , expected * 1.001f);
    }
    drop(p);
}

static void test_widen_S8() {
    Program* p;
    {
        Builder* b = builder();

        V8 x = ld1_8(b);
        st1_16(b, widen_S8(b, x));

        p = compile(b);
    }

    int8_t  in[] = {-128,-1,0,1,127};
    int16_t out[len(in)] = {0};
    run(p,len(in), NULL, (void*[]){in,out});
    for (int i = 0; i < len(in); i++) {
        expect_eq(out[i], in[i]);
    }
    drop(p);
}
static void test_widen_U8() {
    Program* p;
    {
        Builder* b = builder();

        V8 x = ld1_8(b);
        st1_16(b, widen_U8(b, x));

        p = compile(b);
    }

    uint8_t  in[] = {0,128,255};
    uint16_t out[len(in)] = {0};
    run(p,len(in), NULL, (void*[]){in,out});
    for (int i = 0; i < len(in); i++) {
        expect_eq(out[i], in[i]);
    }
    drop(p);
}

static void test_widen_S16() {
    Program* p;
    {
        Builder* b = builder();

        V16 x = ld1_16(b);
        st1_32(b, widen_S16(b, x));

        p = compile(b);
    }

    int16_t  in[] = {-32768,-1,0,1,32767};
    int32_t out[len(in)] = {0};
    run(p,len(in), NULL, (void*[]){in,out});
    for (int i = 0; i < len(in); i++) {
        expect_eq(out[i], in[i]);
    }
    drop(p);
}
static void test_widen_U16() {
    Program* p;
    {
        Builder* b = builder();

        V16 x = ld1_16(b);
        st1_32(b, widen_U16(b, x));

        p = compile(b);
    }

    uint16_t  in[] = {0,32768,65535};
    uint32_t out[len(in)] = {0};
    run(p,len(in), NULL, (void*[]){in,out});
    for (int i = 0; i < len(in); i++) {
        expect_eq(out[i], in[i]);
    }
    drop(p);
}
static void test_widen_F16() {
    Program* p;
    {
        Builder* b = builder();

        V16 x = ld1_16(b);
        st1_32(b, widen_F16(b, x));

        p = compile(b);
    }

    __fp16 in[] = {-INFINITY,-1,0,+1,+INFINITY};
    float out[len(in)] = {0};
    run(p,len(in), NULL, (void*[]){in,out});
    for (int i = 0; i < len(in); i++) {
        expect_eq(out[i], (float)in[i]);
    }
    drop(p);
}

static void test_narrow_F32() {
    Program* p;
    {
        Builder* b = builder();

        V32 x = ld1_32(b);
        st1_16(b, narrow_F32(b, x));

        p = compile(b);
    }

    float in[] = {-INFINITY,-1,0,+1,+INFINITY};
    __fp16 out[len(in)] = {0};
    run(p,len(in), NULL, (void*[]){in,out});
    for (int i = 0; i < len(in); i++) {
        expect_eq((float)out[i], in[i]);
    }
    drop(p);
}
static void test_narrow_I32() {
    Program* p;
    {
        Builder* b = builder();

        V32 x = ld1_32(b);
        st1_16(b, narrow_I32(b, x));

        p = compile(b);
    }

    int32_t in[] = {-32768,-1,0,1,32767};
    int16_t out[len(in)] = {0};
    run(p,len(in), NULL, (void*[]){in,out});
    for (int i = 0; i < len(in); i++) {
        expect_eq(out[i], in[i]);
    }
    drop(p);
}
static void test_narrow_I16() {
    Program* p;
    {
        Builder* b = builder();

        V16 x = ld1_16(b);
        st1_8(b, narrow_I16(b, x));

        p = compile(b);
    }

    int16_t in[] = {-128,-1,0,1,127};
    int8_t out[len(in)] = {0};
    run(p,len(in), NULL, (void*[]){in,out});
    for (int i = 0; i < len(in); i++) {
        expect_eq(out[i], in[i]);
    }
    drop(p);
}


void goal(uint32_t dst[], uint32_t val, int n);

__attribute__((noinline))
void goal(uint32_t dst[], uint32_t val, int n) {
    #define N 8
    typedef uint32_t __attribute__((ext_vector_type(N), aligned(4))) Wide;
    for (; n >= N; n -= N) {
        *(Wide*)dst = val;
        dst += N;
    }
    while (n --> 0) {
        *dst++ = val;
    }
}

static double memset32_goal(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    double start = now();
    uint32_t buf[1024];
    while (k --> 0) {
        goal(buf, 0xffaaccee, len(buf));
    }
    return now() - start;
}

static double memset32_splat(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    Program* p;
    {
        Builder* b = builder();
        V32   v = splat_32(b, (int)0xffaaccee);
        st1_32(b, v);
        p = compile(b);
    }

    double start = now();
    uint32_t buf[1024];
    while (k --> 0) {
        run(p, len(buf), NULL, (void*[]){buf});
    }
    double elapsed = now() - start;
    drop(p);
    return elapsed;
}

static double memset32_uniform(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    Program* p;
    {
        Builder* b = builder();
        V32   v = uniform_32(b, 3);
        st1_32(b, v);
        p = compile(b);
    }

    uint8_t uni[] =  { 0,1,2,0xee,0xcc,0xaa,0xff,4 };
    uint32_t buf[1024];

    double start = now();
    while (k --> 0) {
        run(p, len(buf), uni, (void*[]){buf});
    }
    double elapsed = now() - start;

    drop(p);
    return elapsed;
}

static double swap_rb(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    Program* p;
    {
        Builder* b = builder();

        struct V8x4 px = ld4_8(b);
        st4_8(b, px.b, px.g, px.r, px.a);

        p = compile(b);
    }

    uint32_t buf[1024];

    double start = now();
    while (k --> 0) {
        run(p, len(buf), NULL, (void*[]){buf,buf});
    }
    double elapsed = now() - start;

    drop(p);
    return elapsed;
}

static double compile_memset32(int k, double *scale, const char* *unit) {
    *scale = 1;
    *unit  = "program";

    double start = now();
    while (k --> 0) {
        Builder* b = builder();
        V32   v = splat_32(b, (int)0xffaaccee);
        st1_32(b, v);
        drop(compile(b));
    }

    return now() - start;
}

static double compile_cse(int k, double *scale, const char* *unit) {
    *scale = 1;
    *unit  = "program";

    double start = now();
    while (k --> 0) {
        Builder* b = builder();

        V32 x =   ld1_32(b),
            y = splat_32(b, 3),
            z =  add_I32(b, x,y),
            w =  add_I32(b, x,y);
        st1_32(b, mul_I32(b, z,w));
        drop(compile(b));
    }

    return now() - start;
}

int main(int argc, char** argv) {
    test_nothing();
    test_memset32();
    test_memset32_uniform();
    test_F16();
    test_cse();
    test_dce();
    test_structs();
    test_constant_prop();
    test_peephole();
    test_mul_add_fusion();
    test_mul_sub_fusion();
    test_sqrt();
    test_widen_S8();
    test_widen_U8();
    test_widen_S16();
    test_widen_U16();
    test_widen_F16();
    test_narrow_F32();
    test_narrow_I32();
    test_narrow_I16();

    bench(memset32_goal);
    bench(memset32_splat);
    bench(memset32_uniform);
    bench(swap_rb);
    bench(compile_memset32);
    bench(compile_cse);
    return 0;
}
