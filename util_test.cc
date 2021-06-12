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

    expect(len == 0);
    expect(ptr == nullptr);

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < i; j++) {
            push(ptr,len) = j;
        }
        for (int j = i; j --> 0;) {
            expect(pop(ptr,len) == j);
        }
        expect(len == 0);
        expect(ptr == nullptr);
    }

    return 0;
}
