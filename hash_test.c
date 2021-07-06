#include "expect.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>

static _Bool str_eq(const void* a, const void* b) {
    return 0 == strcmp(a,b);
}

static void test_basics() {
    Hash h = {.eq=str_eq};
    expect(lookup(&h,  42,  "foo") == NULL);
    expect(lookup(&h,  23,  "bar") == NULL);
    expect(lookup(&h,  47,  "baz") == NULL);
    expect(lookup(&h, 100, "quux") == NULL);

    int x = 0;
    insert(&h, 42, "foo", &x);
    expect_eq(h.len, 1);
    expect_eq(h.cap, 1);
    expect(lookup(&h,  42,  "foo") == &x);
    expect(lookup(&h,  23,  "bar") == NULL);
    expect(lookup(&h,  47,  "baz") == NULL);
    expect(lookup(&h, 100, "quux") == NULL);

    int y = 0;
    insert(&h, 23, "bar", &y);
    expect_eq(h.len, 2);
    expect_eq(h.cap, 2);
    expect(lookup(&h,  42,  "foo") == &x);
    expect(lookup(&h,  23,  "bar") == &y);
    expect(lookup(&h,  47,  "baz") == NULL);
    expect(lookup(&h, 100, "quux") == NULL);

    int z = 0;
    insert(&h, 47, "baz", &z);
    expect_eq(h.len, 3);
    expect_eq(h.cap, 4);
    expect(lookup(&h,  42,  "foo") == &x);
    expect(lookup(&h,  23,  "bar") == &y);
    expect(lookup(&h,  47,  "baz") == &z);
    expect(lookup(&h, 100, "quux") == NULL);

    insert(&h, 47, "baz", &x);
    expect_eq(h.len, 3);
    expect_eq(h.cap, 4);
    expect(lookup(&h,  42,  "foo") == &x);
    expect(lookup(&h,  23,  "bar") == &y);
    expect(lookup(&h,  47,  "baz") == &x);
    expect(lookup(&h, 100, "quux") == NULL);

    int w = 0;
    insert(&h, 100, "quux", &w);
    expect_eq(h.len, 4);
    expect_eq(h.cap, 4);
    expect(lookup(&h,  42,  "foo") == &x);
    expect(lookup(&h,  23,  "bar") == &y);
    expect(lookup(&h,  47,  "baz") == &x);
    expect(lookup(&h, 100, "quux") == &w);

    free(h.table);
}

static void test_update() {
    Hash h = {0};
    for (int i = 0; i < 10; i++) {
        insert(&h, 42,&h,&h);
        expect_eq(h.len, 1);
        expect_eq(h.cap, 1);
    }
    free(h.table);
}

int main(void) {
    test_basics();
    test_update();
    return 0;
}
