#include "bench.h"
#include "len.h"
#include "vm.h"
#include <string.h>

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
    bench(memset32_native);
    bench(memset32_goal);
    bench(memset32_vm);
    bench(compile_memset32);
    bench(compile_cse);
    return 0;
}
