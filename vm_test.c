#include "bench.h"
#include "expect.h"
#include "len.h"
#include "vm.h"
#include <string.h>
#include <sys/mman.h>

static void test_memset32() {
    Program* p;
    {
        Builder* b = builder();
        Ptr buf = arg(b, 4);
        U32   v = splat_U32(b, 0xffaaccee);
        st1_U32(b, buf, v);

        p = compile(b);
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
    Program* p;
    {
        Builder* b = builder();
        Ptr t = arg(b,0),
            d = arg(b,4);

        S32 x = ld1_S32(b,t);
        x = splat_S32(b, 0x42);
        st1_S32(b, d, x);

        p = compile(b);
    }

    void* trap = mmap(NULL,4096, PROT_NONE,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
    expect(trap != (void*)-1);

    int32_t xs[63];
    run(p,len(xs),(void*[]){trap,xs});
    for (int i = 0; i < len(xs); i++) {
        expect_eq(xs[i], 0x42);
    }

    munmap(trap,4096);
}

static void test_structs() {
    Program* p;
    {
        Builder* b = builder();
        Ptr ptr = arg(b,4);

        U8x4 rgba = ld4_U8(b,ptr);

        U8 tmp = rgba.r;
        rgba.r = rgba.b;
        rgba.b = tmp;

        st4_U8(b,ptr,rgba);

        p = compile(b);
    }

    uint32_t px[63];
    for (int i = 0; i < len(px); i++) {
        px[i] = (uint32_t)((i+0) <<  0)
              | (uint32_t)((i+1) <<  8)
              | (uint32_t)((i+2) << 16)
              | (uint32_t)((i+3) << 24);
    }
    run(p,len(px),(void*[]){px});
    for (int i = 0; i < len(px); i++) {
        expect_eq(px[i], (uint32_t)((i+2) <<  0)
                       | (uint32_t)((i+1) <<  8)
                       | (uint32_t)((i+0) << 16)
                       | (uint32_t)((i+3) << 24));
    }
}


void approx_jit(uint32_t dst[], uint32_t val, int n);

__attribute__((noinline))
void approx_jit(uint32_t dst[], uint32_t val, int n) {
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


static double memset32_native(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    double start = now();
    uint32_t buf[1024];
    while (k --> 0) {
        uint32_t p = 0xffaaccee;
        memset_pattern4(buf, &p, sizeof buf);
    }
    return now() - start;
}

static double memset32_goal(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    double start = now();
    uint32_t buf[1024];
    while (k --> 0) {
        approx_jit(buf, 0xffaaccee, len(buf));
    }
    return now() - start;
}

static double memset32_vm(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    Program* p;
    {
        Builder* b = builder();
        Ptr buf = arg(b, 4);
        U32   v = splat_U32(b, 0xffaaccee);
        st1_U32(b, buf, v);
        p = compile(b);
    }

    double start = now();
    uint32_t buf[1024];
    while (k --> 0) {
        run(p, len(buf), (void*[]){buf});
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
        Ptr uni = arg(b, 0),
            buf = arg(b, 4);
        U32   v = uniform_U32(b, uni, 3);
        st1_U32(b, buf, v);
        p = compile(b);
    }

    uint8_t uni[] =  { 0,1,2,0xee,0xcc,0xaa,0xff,4 };
    uint32_t buf[1024];

    double start = now();
    while (k --> 0) {
        run(p, len(buf), (void*[]){uni,buf});
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
        Ptr ptr = arg(b,4);

        U8x4 rgba = ld4_U8(b,ptr);

        U8 tmp = rgba.r;
        rgba.r = rgba.b;
        rgba.b = tmp;

        st4_U8(b,ptr,rgba);

        p = compile(b);
    }

    uint32_t buf[1024];

    double start = now();
    while (k --> 0) {
        run(p, len(buf), (void*[]){buf});
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
        Ptr buf = arg(b, 4);
        U32   v = splat_U32(b, 0xffaaccee);
        st1_U32(b, buf, v);
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
        Ptr ptr = arg(b,4);

        S32 x =   ld1_S32(b, ptr),
            y = splat_S32(b, 3),
            z =   add_S32(b, x,y),
            w =   add_S32(b, x,y);
        st1_S32(b, ptr, mul_S32(b, z,w));
        drop(compile(b));
    }

    return now() - start;
}

int main(int argc, char** argv) {
    test_memset32();
    test_memset32_uniform();
    test_add_F16();
    test_cse();
    test_dce();
    test_structs();

    bench(memset32_native);
    bench(memset32_goal);
    bench(memset32_vm);
    bench(memset32_uniform);
    bench(swap_rb);
    bench(compile_memset32);
    bench(compile_cse);
    return 0;
}
