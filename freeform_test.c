#include "freeform.h"
#include "test.h"

#pragma GCC diagnostic ignored "-Wfloat-equal"

int main(void) {
#if defined(__aarch64__)
    float x[] = { 1,2,3,4,5,6,7,8 };
    float y[] = { 1,1,1,1,1,1,1,1 };

    void (*program[])(void) = {
        ptr, loadX,
        ptr, loadY,
        add,
        ptr, store,
        done,
    };

    interp(program, (void*[]){x,y,x}, 0);

    for (int i = 0; i < 8; i++) {
        expect_eq(x[i], (float)i+2.0f);
    }
#endif
    return 0;
}
