#include "expect.h"
#include "util.h"

int main(int, char**) {
    using namespace mtk;

    int* ptr = nullptr;
    int  len = 0;

    push(ptr,len) = 1;
    push(ptr,len) = 2;
    push(ptr,len) = 3;
    push(ptr,len) = 4;
    expect(len == 4);

    expect(pop(ptr,len) == 4);
    expect(pop(ptr,len) == 3);
    expect(pop(ptr,len) == 2);
    expect(pop(ptr,len) == 1);

    expect(ptr == nullptr);
    expect(len == 0);

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < i; j++) {
            push(ptr,len) = j;
        }
        for (int j = i; j --> 0;) {
            expect(pop(ptr,len) == j);
        }
        expect(ptr == nullptr);
        expect(len == 0);
    }

    float* f  = nullptr;
    size_t fs = 0;

    push(f,fs) = 1.0f;
    expect(fs == 1);
    expect(pop(f,fs) == 1.0f);

    expect(f  == nullptr);
    expect(fs == 0);

    expect(bit_pun<float>(0x3f80'0000) == 1.0f);
    expect(bit_pun<int  >(1.0f) == 0x3f80'0000);

    return 0;
}
