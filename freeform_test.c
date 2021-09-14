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

    typedef void (*jit_fn)(void);

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
        float x[] = { 1,2,3,4,5,6,7,8,9,10 };
        float y[] = { 1,1,1,1,1,1,1,1,1, 1 };
        float z[len(x)] = {0};

        void (*program[])(void) = {
            ptr1, loadX, inc, update1,
            ptr2, loadY, inc, update2,
            add,
            ptr3, store, inc, update3,
            done,
        };

        void (*program1[])(void) = {
            ptr1, loadX1, inc1, update1,
            ptr2, loadY1, inc1, update2,
            add,
            ptr3, store1, inc1, update3,
            done,
        };

        args(0, x+0,y+0,z+0, 0,0,0,0);
        interp(program);
        interp(program1);
        interp(program1);
        for (int i = 0; i < len(x); i++) {
            expect_eq(z[i], x[i]+y[i]);
        }

        for (int i = 0; i < len(x); i++) {
            z[i] = 0;
        }

        jit_fn p8 = jit(program),
               p1 = jit(program1);
        args(0, x+0,y+0,z+0, 0,0,0,0);
        p8();
        p1();
        p1();
        for (int i = 0; i < len(x); i++) {
            expect_eq(z[i], x[i]+y[i]);
        }

        drop_jit(p8);
        drop_jit(p1);

        return 0;
    }
#else
    int main(void) { return 0; }
#endif
