#include "asm.h"
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

        uint32_t* entry = mmap(NULL,size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1,0);
        expect_ne((uintptr_t)entry, ~(uintptr_t)0);

        uint32_t* buf = entry;
        const uint32_t ret = xret(lr);
        while (*program != done) {
            for (const uint32_t* inst = (const uint32_t*)*program++; *inst != ret; ) {
                *buf++ = *inst++;
            }
        }
        *buf++ = xsubs(x1,x1,1);
        uint32_t bne_entry = xbdot(ne, (int)(entry-buf));
        *buf++ = bne_entry;
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

        interp(program_1,len(x), x,y,z, 0,0,0);
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

        jit_fn jit_1 = jit(program_1);

        jit_1(0,len(x), x,y,z, 0,0,0);
        for (int i = 0; i < len(x); i++) {
            expect_eq(z[i], x[i]+y[i]);
            z[i] = 0;
        }

        jit_fn jit_8 = jit(program_8);

        jit_8(0, 2, x+ 0,y+ 0,z+ 0, 0,0,0);
        jit_1(0, 4, x+16,y+16,z+16, 0,0,0);
        for (int i = 0; i < len(x); i++) {
            expect_eq(z[i], x[i]+y[i]);
            z[i] = 0;
        }

        drop_jit(jit_1);
        drop_jit(jit_8);

        return 0;
    }
#else
    int main(void) { return 0; }
#endif
