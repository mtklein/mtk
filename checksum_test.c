#include "checksum.h"
#include "expect.h"

static void test_crc32() {
    expect_eq(0x00000000, crc32(NULL,0));

    int x = 0;
    expect_eq(0x2144df1c, crc32(&x,sizeof x));

    expect_eq(0xe8b7be43, crc32("a", 1));
}

static void test_fnv1a() {
    expect_eq(0x811c9dc5, fnv1a(NULL,0));

    int x = 0;
    expect_eq(0x4b95f515, fnv1a(&x,sizeof x));

    expect_eq(0xe40c292c, fnv1a("a", 1));
}

static void test_murmur3() {
    expect_eq(0x00000000, murmur3(         0, NULL,0));
    expect_eq(0x514e28b7, murmur3(         1, NULL,0));
    expect_eq(0x81f16f39, murmur3(0xffffffff, NULL,0));

    expect_eq(murmur3(         1, NULL,0), mix(         1));
    expect_eq(murmur3(0xffffffff, NULL,0), mix(0xffffffff));

    unsigned x = 0xffffffff;
    expect_eq(0x76293b50, murmur3(0, &x,sizeof x));

    x = 0x87654321;
    expect_eq(0xf55b516b, murmur3(0, &x,4));
    expect_eq(0x7e4a8634, murmur3(0, &x,3));
    expect_eq(0xa0f7b07a, murmur3(0, &x,2));
    expect_eq(0x72661cf4, murmur3(0, &x,1));
}


int main(void) {
    test_crc32();
    test_fnv1a();
    test_murmur3();
    return 0;
}
