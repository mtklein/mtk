#include "len.h"
#include "stack.h"
#include "test.h"

#pragma GCC diagnostic ignored "-Wfloat-equal"

static void test_basics() {
    float x[] = { 4,5 };

    Op* program[] = {load,load,mul,store,done};
    run(program, (void*[]){x,x,x}, len(x));

    expect_eq(x[0], 16);
    expect_eq(x[1], 25);
}

static void test_memset32() {
    float src = 42;
    float dst[19];

    Op* program[] = {uniform,store,done};
    run(program, (void*[]){&src,dst}, len(dst));
}

static double memset32(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    uint32_t src = 42;
    uint32_t dst[1024];

    Op* program[] = {uniform,store,done};

    double start = now();
    while (k --> 0) {
        run(program, (void*[]){&src,dst}, len(dst));
    }
    return now() - start;
}

int main(int argc, char** argv) {
    test_basics();
    test_memset32();

    bench(memset32);
    return 0;
}
