#include "util.h"
#include <assert.h>

int main(int, char**) {
    using namespace mtk;

    int* ptr = nullptr;
    int  len = 0;

    push(ptr,len) = 1;
    push(ptr,len) = 2;
    push(ptr,len) = 3;
    push(ptr,len) = 4;
    assert(len == 4);

    assert(pop(ptr,len) == 4);
    assert(pop(ptr,len) == 3);
    assert(pop(ptr,len) == 2);
    assert(pop(ptr,len) == 1);

    assert(len == 0);
    assert(ptr == nullptr);

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < i; j++) {
            push(ptr,len) = j;
        }
        for (int j = i; j --> 0;) {
            assert(pop(ptr,len) == j);
        }
        assert(len == 0);
        assert(ptr == nullptr);
    }

    return 0;
}
