#include "asm.h"
#include "expect.h"

// $ echo "fmul v4.8h, v3.8h, v1.8h" | brew/opt/llvm/bin/llvm-mc -show-encoding -mattr=+fullfp16

int main(void) {
    expect_eq(0xd65f03c0, xret(lr));

    expect_eq(0x91001042, xadd(x2,x2,4));
    expect_eq(0xd1001042, xsub(x2,x2,4));

    expect_eq(0x2e205864, vnot(_8b,  v4,v3));
    expect_eq(0x6e205864, vnot(_16b, v4,v3));

    expect_eq(0x4e61d464, vfadd(_2d, v4,v3,v1));
    expect_eq(0x4e21d464, vfadd(_4s, v4,v3,v1));
    expect_eq(0x0e21d464, vfadd(_2s, v4,v3,v1));
    expect_eq(0x4e411464, vfadd(_8h, v4,v3,v1));
    expect_eq(0x0e411464, vfadd(_4h, v4,v3,v1));

    expect_eq(0x6e61dc64, vfmul(_2d, v4,v3,v1));
    expect_eq(0x6e21dc64, vfmul(_4s, v4,v3,v1));
    expect_eq(0x2e21dc64, vfmul(_2s, v4,v3,v1));
    expect_eq(0x6e411c64, vfmul(_8h, v4,v3,v1));
    expect_eq(0x2e411c64, vfmul(_4h, v4,v3,v1));


    return 0;
}
