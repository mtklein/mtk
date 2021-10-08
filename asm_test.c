#include "asm.h"
#include "test.h"

#if defined(__aarch64__)
    #include <sys/mman.h>
    #include <unistd.h>
#endif

static void jit(void(*setup)(uint32_t*),
                void(*test )(void(*)(void))) {
#if defined(__aarch64__)
    const size_t size = (size_t)sysconf(_SC_PAGESIZE);  // One page ought to be enough for anyone.

    void* p = mmap(NULL,size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1,0);
    expect_ne((uintptr_t)p, ~(uintptr_t)0);
    setup(p);

    expect_eq(0, mprotect(p,size, PROT_READ|PROT_EXEC));
    test((void(*)(void))p);

    munmap(p,size);
#else
    (void)setup;
    (void)test;
#endif
}

static void setup_square(uint32_t* p) {
    *p++ = vfmul(Arr_4s, v0,v0,v0);
    *p++ = xret(lr);
}
static void test_square(void(*p)(void)) {
    float (*fn)(float) = (float(*)(float))p;
    expect_eq(9.0f, fn(3.0f));
}

// echo "fmul v4.8h, v3.8h, v1.8h" | ~/brew/opt/llvm/bin/llvm-mc -show-encoding -mattr=+fullfp16

int main(void) {
    expect_eq(0xd65f03c0, xret(lr));
    expect_eq(0x54fffda1, xbdot(ne, -19));   // b.ne -76

    expect_eq(0x91001042, xadd (x2,x2,4));
    expect_eq(0xd1001042, xsub (x2,x2,4));
    expect_eq(0xf1001042, xsubs(x2,x2,4));

    expect_eq(0x2e205864, vnot(Arr_8b,  v4,v3));
    expect_eq(0x6e205864, vnot(Arr_16b, v4,v3));

    expect_eq(0x4e61d464, vfadd(Arr_2d, v4,v3,v1));
    expect_eq(0x4e21d464, vfadd(Arr_4s, v4,v3,v1));
    expect_eq(0x0e21d464, vfadd(Arr_2s, v4,v3,v1));
    expect_eq(0x4e411464, vfadd(Arr_8h, v4,v3,v1));
    expect_eq(0x0e411464, vfadd(Arr_4h, v4,v3,v1));

    expect_eq(0x6e61dc64, vfmul(Arr_2d, v4,v3,v1));
    expect_eq(0x6e21dc64, vfmul(Arr_4s, v4,v3,v1));
    expect_eq(0x2e21dc64, vfmul(Arr_2s, v4,v3,v1));
    expect_eq(0x6e411c64, vfmul(Arr_8h, v4,v3,v1));
    expect_eq(0x2e411c64, vfmul(Arr_4h, v4,v3,v1));

    jit(setup_square, test_square);

    return 0;
}
