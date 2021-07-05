#include "expect.h"
#include "len.h"
#include "vm.h"

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

int main(void) {
    test_memset32();
    test_add_F16();
    return 0;
}
