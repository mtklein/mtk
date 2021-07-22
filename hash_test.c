#include "expect.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char* *keys;
    const char*  key;
} str_eq_ctx;

static bool str_eq(int val, void* vctx) {
    const str_eq_ctx* ctx = vctx;
    return 0 == strcmp(ctx->key, ctx->keys[val]);
}

static void test_basics() {
    static const char* keys[] = { "foo", "bar", "baz", "quux" };
    Hash h = {0};

    str_eq_ctx ctx = {.keys=keys};

    int val;
    ctx.key =  "foo"; expect(!lookup(&h,  42, str_eq, &ctx, &val));
    ctx.key =  "bar"; expect(!lookup(&h,  23, str_eq, &ctx, &val));
    ctx.key =  "baz"; expect(!lookup(&h,  47, str_eq, &ctx, &val));
    ctx.key = "quux"; expect(!lookup(&h, 100, str_eq, &ctx, &val));

    insert(&h, 42, 0);
    expect_eq(h.len, 1);
    expect_eq(h.cap, 1);
    ctx.key =  "foo"; expect( lookup(&h,  42, str_eq, &ctx, &val) && val == 0);
    ctx.key =  "bar"; expect(!lookup(&h,  23, str_eq, &ctx, &val));
    ctx.key =  "baz"; expect(!lookup(&h,  47, str_eq, &ctx, &val));
    ctx.key = "quux"; expect(!lookup(&h, 100, str_eq, &ctx, &val));

    insert(&h, 23, 1);
    expect_eq(h.len, 2);
    expect_eq(h.cap, 2);
    ctx.key =  "foo"; expect( lookup(&h,  42, str_eq, &ctx, &val) && val == 0);
    ctx.key =  "bar"; expect( lookup(&h,  23, str_eq, &ctx, &val) && val == 1);
    ctx.key =  "baz"; expect(!lookup(&h,  47, str_eq, &ctx, &val));
    ctx.key = "quux"; expect(!lookup(&h, 100, str_eq, &ctx, &val));

    insert(&h, 47, 2);
    expect_eq(h.len, 3);
    expect_eq(h.cap, 4);
    ctx.key =  "foo"; expect( lookup(&h,  42, str_eq, &ctx, &val) && val == 0);
    ctx.key =  "bar"; expect( lookup(&h,  23, str_eq, &ctx, &val) && val == 1);
    ctx.key =  "baz"; expect( lookup(&h,  47, str_eq, &ctx, &val) && val == 2);
    ctx.key = "quux"; expect(!lookup(&h, 100, str_eq, &ctx, &val));

    insert(&h, 100, 3);
    expect_eq(h.len, 4);
    expect_eq(h.cap, 8);
    ctx.key =  "foo"; expect( lookup(&h,  42, str_eq, &ctx, &val) && val == 0);
    ctx.key =  "bar"; expect( lookup(&h,  23, str_eq, &ctx, &val) && val == 1);
    ctx.key =  "baz"; expect( lookup(&h,  47, str_eq, &ctx, &val) && val == 2);
    ctx.key = "quux"; expect( lookup(&h, 100, str_eq, &ctx, &val) && val == 3);

    free(h.table);
}

static void test_growth() {
    Hash h = {0};

    const int expected[] = {1,2,4,8,8,8,16,16,16,16,16,16,32,32,32,32,32,32,32,32};
    for (int i = 0; i < 20; i++) {
        insert(&h,i,i);
        expect_eq(h.cap, expected[i]);
    }
    free(h.table);
}

int main(void) {
    test_basics();
    test_growth();
    return 0;
}
