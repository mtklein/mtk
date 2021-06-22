#include "ns.h"

static int add(int x, int y) { return x+y; }
static int mul(int x, int y) { return x*y; }

struct ns ns = {
    .constantA = 42,
    .constantB = 47,
    .add = add,
    .mul = mul,
};
