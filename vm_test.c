#include "expect.h"
#include "len.h"
#include "vm.h"

static void test_memset32() {
    Program* p;
    {
        Builder* b = builder();
        Ptr buf = arg(b, 4);
        U32   v = splat_U32(b, 0xffaaccee);
        st1_32(b, buf, v);
        p = compile(b);
    }

    uint32_t buf[63] = {0};
    run(p, len(buf), (void*[]){buf});

    for (int i = 0; i < len(buf); i++) {
        expect(buf[i] == 0xffaaccee);
    }

    drop(p);
}

int main(void) {
    test_memset32();
    return 0;
}
