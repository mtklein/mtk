#include "stack.h"

void done(void* op[], void* data[], int n,
          V s0, V s1, V s2, V s3, V s4, V s5, V s6, V s7) {
    (void)op;
    (void)data;
    (void)n;
    (void)s0;
    (void)s1;
    (void)s2;
    (void)s3;
    (void)s4;
    (void)s5;
    (void)s6;
    (void)s7;
}

#define next ((Op*)*op)(op+1,data,n, s0,s1,s2,s3, s4,s5,s6,s7)
#define push s7=s6; s6=s5; s5=s4; s4=s3; s3=s2; s2=s1; s1=s0; s0 =
#define pop  s0; s0=s1; s1=s2; s2=s3; s3=s4; s4=s5; s5=s6; s6=s7

void load(void* op[], void* data[], int n,
          V s0, V s1, V s2, V s3, V s4, V s5, V s6, V s7) {
    if (n<N) { float* ptr = *data; push *ptr++; *data++ = ptr; }
    else     { V*     ptr = *data; push *ptr++; *data++ = ptr; }
    next;
}

void uniform(void* op[], void* data[], int n,
             V s0, V s1, V s2, V s3, V s4, V s5, V s6, V s7) {
    float* ptr = *data++;
    push *ptr;
    next;
}

void store(void* op[], void* data[], int n,
           V s0, V s1, V s2, V s3, V s4, V s5, V s6, V s7) {
    V x = pop;
    if (n<N) { float* ptr = *data; *ptr++ = x[0]; *data++ = ptr; }
    else     { V*     ptr = *data; *ptr++ = x   ; *data++ = ptr; }
    next;
}

void add(void* op[], void* data[], int n,
         V s0, V s1, V s2, V s3, V s4, V s5, V s6, V s7) {
    V x = pop;
    V y = pop;
    push x+y;
    next;
}

void mul(void* op[], void* data[], int n,
         V s0, V s1, V s2, V s3, V s4, V s5, V s6, V s7) {
    V x = pop;
    V y = pop;
    push x*y;
    next;
}

void run(Op* op[], void* data[], int n) {
    V u[1];
    for (; n; n -= (n<N ? 1 : N)) {
        (*op)((void**)op+1, data, n,
              u[0],u[0],u[0],u[0], u[0],u[0],u[0],u[0]);
    }
}
