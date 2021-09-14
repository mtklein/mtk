#include "freeform.h"
#include "len.h"
#include "test.h"
#include <stdint.h>

#if defined(__aarch64__)
    #include <sys/mman.h>
    #include <unistd.h>
#endif

#pragma GCC diagnostic ignored "-Wfloat-equal"

#if defined(__aarch64__)

    typedef void (*jit_fn)(int unused, int n, void* A, void* B, void* C
                                            , void* D, void* E, void* F);

    static jit_fn jit(void (*program[])(void)) {
        const size_t size = (size_t)sysconf(_SC_PAGESIZE);

        void* entry = mmap(NULL,size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1,0);
        expect_ne((uintptr_t)entry, ~(uintptr_t)0);

        uint32_t* buf = entry;
        const uint32_t ret = 0xd65f03c0;
        while (*program != done) {
            for (const uint32_t* inst = (const uint32_t*)*program++; *inst != ret; ) {
                *buf++ = *inst++;
            }
        }
        *buf++ = ret;

        expect_eq(0, mprotect(entry,size, PROT_READ|PROT_EXEC));
        return (jit_fn)entry;
    }

    static void drop_jit(jit_fn fn) {
        const size_t size = (size_t)sysconf(_SC_PAGESIZE);
        munmap((void*)fn,size);
    }


    int main(void) {
        float x[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
        float y[] = { 1,1,1,1,1,1,1,1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        float z[len(x)] = {0};

        void (*program_1[])(void) = {
            ptrA, loadX_1, incA,
            ptrB, loadY_1, incB,
            add,
            ptrC, store_1, incC,
            done,
        };
        interp(program_1,len(x), x+0,y+0,z+0, 0,0,0);
        for (int i = 0; i < len(x); i++) {
            expect_eq(z[i], x[i]+y[i]);
            z[i] = 0;
        }

        void (*program_8[])(void) = {
            ptrA, loadX_8, incA,
            ptrB, loadY_8, incB,
            add,
            ptrC, store_8, incC,
            done,
        };

        interp(program_8,2, x+ 0,y+ 0,z+ 0, 0,0,0);
        interp(program_1,4, x+16,y+16,z+16, 0,0,0);
        for (int i = 0; i < len(x); i++) {
            expect_eq(z[i], x[i]+y[i]);
            z[i] = 0;
        }

        jit_fn p8 = jit(program_8),
               p1 = jit(program_1);
        p8(0, 0, x+ 0,y+ 0,z+ 0, 0,0,0);
        p8(0, 0, x+ 8,y+ 8,z+ 8, 0,0,0);
        p1(0, 0, x+16,y+16,z+16, 0,0,0);
        p1(0, 0, x+17,y+17,z+17, 0,0,0);
        p1(0, 0, x+18,y+18,z+18, 0,0,0);
        p1(0, 0, x+19,y+19,z+19, 0,0,0);
        for (int i = 0; i < len(x); i++) {
            expect_eq(z[i], x[i]+y[i]);
            z[i] = 0;
        }

        drop_jit(p8);
        drop_jit(p1);

        return 0;
    }
#else
    int main(void) { return 0; }
#endif
