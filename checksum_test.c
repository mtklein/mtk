#include "checksum.h"
#include "expect.h"

static void test_murmur3() {
    expect_eq(murmur3(         0, NULL,0), 0);
    expect_eq(murmur3(         1, NULL,0), 0x514e28b7);
    expect_eq(murmur3(0xffffffff, NULL,0), 0x81f16f39);

    expect_eq(murmur3(         1, NULL,0), mix(         1));
    expect_eq(murmur3(0xffffffff, NULL,0), mix(0xffffffff));

    unsigned x = 0xffffffff;
    expect_eq(murmur3(0, &x,sizeof x), 0x76293B50);

    x = 0x87654321;
    expect_eq(murmur3(0, &x,4), 0xf55b516b);
    expect_eq(murmur3(0, &x,3), 0x7e4a8634);
    expect_eq(murmur3(0, &x,2), 0xa0f7b07a);
    expect_eq(murmur3(0, &x,1), 0x72661cf4);
}

int main(void) {
    test_murmur3();
    return 0;
}
