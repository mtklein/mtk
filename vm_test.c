#include "expect.h"
#include "len.h"
#include "vm.h"
#include <string.h>

static int vals(const Program* p) {
    int v;
    memcpy(&v, (const char*)p+0, sizeof v);
    return v;
}
static int loop(const Program* p) {
    int v;
    memcpy(&v, (const char*)p+4, sizeof v);
    return v;
}

static void test_memset32() {
    Program* p;
    {
        Builder* b = builder();
        Ptr buf = arg(b, 4);
        U32   v = splat_U32(b, 0xffaaccee);
        st1_U32(b, buf, v);

        p = compile(b);
        expect_eq(vals(p), 2);
        expect_eq(loop(p), 1);
    }

    uint32_t buf[63] = {0};
    run(p, len(buf), (void*[]){buf});

    for (int i = 0; i < len(buf); i++) {
        expect_eq(buf[i], 0xffaaccee);
    }

    drop(p);
}

static void test_memset32_uniform() {
    Program* p;
    {
        Builder* b = builder();
        Ptr uni = arg(b,0),
            buf = arg(b,4);
        U32   v = uniform_U32(b, uni, 3);
        st1_U32(b, buf, v);

        p = compile(b);
        expect_eq(vals(p), 2);
        expect_eq(loop(p), 1);
    }

    uint8_t uni[] =  { 0,1,2,0xee,0xcc,0xaa,0xff,4 };
    uint32_t buf[63] = {0};
    run(p, len(buf), (void*[]){uni, buf});

    for (int i = 0; i < len(buf); i++) {
        expect_eq(buf[i], 0xffaaccee);
    }

    drop(p);
}

static void test_add_F16() {
    Program* p;
    {
        Builder* b = builder();
        Ptr xp = arg(b,2),
            yp = arg(b,2);

        F16 x = ld1_F16(b,xp),
            y = ld1_F16(b,yp),
            z = add_F16(b, x,y);
        st1_F16(b, xp, z);

        p = compile(b);
        expect_eq(vals(p), 4);
        expect_eq(loop(p), 0);
    }

    _Float16 x[63],
             y[len(x)];

    for (int i = 0; i < len(x); i++) {
        x[i] = 0.125f16;
        y[i] = 0.250f16;
    }

    run(p,len(x), (void*[]){x,y});
    for (int i = 0; i < len(x); i++) {
        expect_eq(x[i], 0.375f16);
        expect_eq(y[i], 0.250f16);
    }
}

static void test_cse() {
    Program* p;
    {
        Builder* b = builder();
        Ptr ptr = arg(b,4);

        S32 x =   ld1_S32(b, ptr),
            y = splat_S32(b, 3),
            z =   add_S32(b, x,y),
            w =   add_S32(b, x,y);
        st1_S32(b, ptr, mul_S32(b, z,w));

        expect_eq(z.id, w.id);

        p = compile(b);
        expect_eq(vals(p), 5);
        expect_eq(loop(p), 1);
    }

    int32_t xs[63];
    for (int i = 0; i < len(xs); i++) {
        xs[i] = i;
    }

    run(p,len(xs),(void*[]){xs});

    for (int i = 0; i < len(xs); i++) {
        expect_eq(xs[i], (i+3)*(i+3));
    }
}

static void test_dce() {
    const int K = 17;
    Program* p;
    {
        Builder* b = builder();
        Ptr ptr = arg(b,4);

        S32 x = {0};
        for (int i = 0; i < K; i++) {
            x = splat_S32(b, i);
        }
        st1_S32(b, ptr, x);

        p = compile(b);

        expect_eq(vals(p), 2);
        expect_eq(loop(p), 1);
    }

    int32_t xs[63];

    run(p,len(xs),(void*[]){xs});

    for (int i = 0; i < len(xs); i++) {
        expect_eq(xs[i], K-1);
    }
}

int main(void) {
    test_memset32();
    test_memset32_uniform();
    test_add_F16();
    test_cse();
    test_dce();
    return 0;
}
