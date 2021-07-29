#include "asm.h"
#include "assume.h"

#pragma GCC diagnostic ignored "-Wgnu-binary-literal"

typedef union {
    uint32_t bits;
    struct {
        uint32_t d   :  5;
        uint32_t n   :  5;
        uint32_t l   :  6;
        uint32_t m   :  5;
        uint32_t h11 : 11;
    };
} Op;

typedef union {
    uint32_t bits;
    struct {
        uint32_t d     :  5;
        uint32_t n     :  5;
        uint32_t imm12 : 12;
        uint32_t h10   : 10;
    };
} Op_imm12;

typedef union {
    uint32_t bits;
    struct {
        uint32_t d  : 5;
        uint32_t n  : 5;
        uint32_t l  : 6;
        uint32_t m  : 5;
        uint32_t h9 : 9;
        uint32_t Q  : 1;
        uint32_t t  : 1;
    };
} Op_Q;


static uint32_t mask(int bits, uint32_t v) {
    uint32_t masked = v & ((1<<bits) - 1);
    assume (masked == v);
    return masked;
}

uint32_t xret(X n) { return (Op){.h11=0b11010110010, .m=0b11111, .n=n}.bits; }

uint32_t xadd(X d, X n, unsigned imm12) {
    return (Op_imm12){.h10=0b1001000100, .imm12=mask(12,imm12), .n=n, .d=d}.bits;
}
uint32_t xsub(X d, X n, unsigned imm12) {
    return (Op_imm12){.h10=0b1101000100, .imm12=mask(12,imm12), .n=n, .d=d}.bits;
}

uint32_t vnot(Arr arr, V d, V n) {
    switch (arr>>1) {
        case 0: return (Op_Q){.Q=arr&1, .h9=0b101110001, .m=0, .l=0b010110, .n=n, .d=d}.bits;
    }
    assume(0);
}

uint32_t vfadd(Arr arr, V d, V n, V m) {
    switch (arr>>1) {
        case 1: return (Op_Q){.Q=arr&1, .h9=0b001110010, .m=m, .l=0b000101, .n=n, .d=d}.bits;
        case 2: return (Op_Q){.Q=arr&1, .h9=0b001110001, .m=m, .l=0b110101, .n=n, .d=d}.bits;
        case 3: return (Op_Q){.Q=arr&1, .h9=0b001110011, .m=m, .l=0b110101, .n=n, .d=d}.bits;
    }
    assume(0);
}

uint32_t vfmul(Arr arr, V d, V n, V m) {
    switch (arr>>1) {
        case 1: return (Op_Q){.Q=arr&1, .h9=0b101110010, .m=m, .l=0b000111, .n=n, .d=d}.bits;
        case 2: return (Op_Q){.Q=arr&1, .h9=0b101110001, .m=m, .l=0b110111, .n=n, .d=d}.bits;
        case 3: return (Op_Q){.Q=arr&1, .h9=0b101110011, .m=m, .l=0b110111, .n=n, .d=d}.bits;
    }
    assume(0);
}
