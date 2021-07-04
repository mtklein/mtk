#include "bench.h"
#include "len.h"
#include "vm.h"
#include <string.h>

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

static double memset32_vm(int k, double *scale, const char* *unit) {
    *scale = 1024;
    *unit  = "px";

    Program* p;
    {
        Builder* b = builder();
        Ptr buf = arg(b, 4);
        U32   v = splat_U32(b, 0xffaaccee);
        st1_32(b, buf, v);
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

int main(int argc, char** argv) {
    bench(memset32_native);
    bench(memset32_vm);
    return 0;
}
