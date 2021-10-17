#include "hash.h"
#include "test.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char* *keys;
    const char*  key;
    int          val;
    int          unused;
} str_eq_ctx;

static bool str_eq(int val, void* vctx) {
    str_eq_ctx* ctx = vctx;
    if (0 == strcmp(ctx->key, ctx->keys[val])) {
        ctx->val = val;
        return true;
    }
    return false;
}

static void test_basics() {
    static const char* keys[] = { "foo", "bar", "baz", "quux" };
    Hash h = {0};

    str_eq_ctx ctx = {.keys=keys};

    ctx.key =  "foo"; expect(!lookup(&h,  42, str_eq, &ctx));
    ctx.key =  "bar"; expect(!lookup(&h,  23, str_eq, &ctx));
    ctx.key =  "baz"; expect(!lookup(&h,  47, str_eq, &ctx));
    ctx.key = "quux"; expect(!lookup(&h, 100, str_eq, &ctx));

    insert(&h, 42, 0);
    expect_eq(h.len, 1);
    expect_eq(h.cap, 1);
    ctx.key =  "foo"; expect( lookup(&h,  42, str_eq, &ctx) && ctx.val == 0);
    ctx.key =  "bar"; expect(!lookup(&h,  23, str_eq, &ctx));
    ctx.key =  "baz"; expect(!lookup(&h,  47, str_eq, &ctx));
    ctx.key = "quux"; expect(!lookup(&h, 100, str_eq, &ctx));

    insert(&h, 23, 1);
    expect_eq(h.len, 2);
    expect_eq(h.cap, 2);
    ctx.key =  "foo"; expect( lookup(&h,  42, str_eq, &ctx) && ctx.val == 0);
    ctx.key =  "bar"; expect( lookup(&h,  23, str_eq, &ctx) && ctx.val == 1);
    ctx.key =  "baz"; expect(!lookup(&h,  47, str_eq, &ctx));
    ctx.key = "quux"; expect(!lookup(&h, 100, str_eq, &ctx));

    insert(&h, 47, 2);
    expect_eq(h.len, 3);
    expect_eq(h.cap, 4);
    ctx.key =  "foo"; expect( lookup(&h,  42, str_eq, &ctx) && ctx.val == 0);
    ctx.key =  "bar"; expect( lookup(&h,  23, str_eq, &ctx) && ctx.val == 1);
    ctx.key =  "baz"; expect( lookup(&h,  47, str_eq, &ctx) && ctx.val == 2);
    ctx.key = "quux"; expect(!lookup(&h, 100, str_eq, &ctx));

    insert(&h, 100, 3);
    expect_eq(h.len, 4);
    expect_eq(h.cap, 8);
    ctx.key =  "foo"; expect( lookup(&h,  42, str_eq, &ctx) && ctx.val == 0);
    ctx.key =  "bar"; expect( lookup(&h,  23, str_eq, &ctx) && ctx.val == 1);
    ctx.key =  "baz"; expect( lookup(&h,  47, str_eq, &ctx) && ctx.val == 2);
    ctx.key = "quux"; expect( lookup(&h, 100, str_eq, &ctx) && ctx.val == 3);

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

static bool is_42(int val, void* ctx) {
    (void)ctx;
    return val == 42;
}

static void test_duplicates() {
    Hash h = {0};

    for (int i = 0; i < 7; i++) {
        expect_eq(h.len, i);
        insert(&h, 0,42);
        expect(lookup(&h,0,is_42,NULL));
    }

    free(h.table);
}


static double bench_insert(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};

    double start = now();
    for (int i = 0; i < k; i++) {
        insert(&h,i,i);
    }
    double elapsed = now() - start;
    expect_eq(h.len, k);
    free(h.table);
    return elapsed;
}

static bool always_match(int val, void* vctx) {
    int* ctx = vctx;
    *ctx = val;
    return true;
}

static double bench_hit(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};
    for (int i = 0; i < 128; i++) {
        insert(&h,i,i*2);
    }

    double start = now();
    while (k --> 0) {
        int i = 42, val;
        expect(lookup(&h,i, always_match,&val) && val == 84);
    }
    double elapsed = now() - start;
    free(h.table);
    return elapsed;
}

static double bench_miss(int k, double *scale, const char* *unit) {
    *scale = 1.0;
    *unit  = "";

    Hash h = {0};
    for (int i = 0; i < 128; i++) {
        insert(&h,i,i*2);
    }

    double start = now();
    while (k --> 0) {
        int i = 420;
        expect(!lookup(&h,i, always_match,NULL));
    }
    double elapsed = now() - start;
    free(h.table);
    return elapsed;
}

int main(int argc, char** argv) {
    test_basics();
    test_growth();
    test_duplicates();

    bench(bench_insert);
    bench(bench_hit);
    bench(bench_miss);
    return 0;
}
