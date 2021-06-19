#include "array.h"
#include "expect.h"

int main(void) {

    float* fs = NULL;
    int n = 0;

    push(fs,n) = 1.0f;
    push(fs,n) = 2.0f;
    push(fs,n) = 3.0f;
    push(fs,n) = 4.0f;
    push(fs,n) = 5.0f;

    expect(n == 5);
    expect(fs[0] == 1.0f);
    expect(fs[1] == 2.0f);
    expect(fs[2] == 3.0f);
    expect(fs[3] == 4.0f);
    expect(fs[4] == 5.0f);

    pop(fs,n); expect(n == 4);
    pop(fs,n); expect(n == 3);
    pop(fs,n); expect(n == 2);
    pop(fs,n); expect(n == 1);
    expect(fs[0] == 1.0f);

    pop(fs,n);
    expect(n == 0);
    expect(fs == NULL);

    size_t m = 0;
    push(fs,m) = 42.0f;
    expect(m == 1);
    expect(fs[0] == 42.0f);
    pop(fs,m);
    expect(m == 0);
    expect(fs == NULL);

    return 0;
}
