#include "ns.h"
#include "expect.h"

int main(void) {
    expect(ns.mul(ns.constantB, 12) == 564);
    return 0;
}
