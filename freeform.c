#include "freeform.h"

#if defined(__aarch64__)
__asm__(
    ".align 4"                       "\n"

    "program .req x0"                "\n"

    "ptr     .req x9"                "\n"
    "fn      .req x10"               "\n"
    "stash   .req x11"               "\n"

    ".global _args"                  "\n"
    ".global _interp"                "\n"
    ".global _done"                  "\n"

    ".global _ptr1"                  "\n"
    ".global _ptr2"                  "\n"
    ".global _ptr3"                  "\n"
    ".global _ptr4"                  "\n"
    ".global _ptr5"                  "\n"
    ".global _ptr6"                  "\n"
    ".global _ptr7"                  "\n"

    ".global _update1"               "\n"
    ".global _update2"               "\n"
    ".global _update3"               "\n"
    ".global _update4"               "\n"
    ".global _update5"               "\n"
    ".global _update6"               "\n"
    ".global _update7"               "\n"

    ".global _inc"                   "\n"
    ".global _inc1"                  "\n"

    ".global _store"                 "\n"
    ".global _store1"                "\n"
    ".global _loadX"                 "\n"
    ".global _loadX1"                "\n"
    ".global _loadY"                 "\n"
    ".global _loadY1"                "\n"

    ".global _add"                   "\n"
    ".global _mul"                   "\n"

    "_args: ret"                     "\n"

    "_interp:"                       "\n"
        "mov stash,lr"               "\n"   // save lr
        "bl  loop"                   "\n"   // effectively, lr = loop
    "loop:"                          "\n"
        "ldr fn,[program],8"         "\n"   // fn = *program++
        "br  fn"                     "\n"   // fn(), returning to loop

    "_done: ret stash"               "\n"   // actually return

    "_ptr1: mov ptr,x1\n ret\n"
    "_ptr2: mov ptr,x2\n ret\n"
    "_ptr3: mov ptr,x3\n ret\n"
    "_ptr4: mov ptr,x4\n ret\n"
    "_ptr5: mov ptr,x5\n ret\n"
    "_ptr6: mov ptr,x6\n ret\n"
    "_ptr7: mov ptr,x7\n ret\n"

    "_inc:  add ptr,ptr,32\n ret\n"
    "_inc1: add ptr,ptr,4 \n ret\n"

    "_update1: mov x1,ptr\n ret\n"
    "_update2: mov x2,ptr\n ret\n"
    "_update3: mov x3,ptr\n ret\n"
    "_update4: mov x4,ptr\n ret\n"
    "_update5: mov x5,ptr\n ret\n"
    "_update6: mov x6,ptr\n ret\n"
    "_update7: mov x7,ptr\n ret\n"

    "_store:  stp q0,q4, [ptr]\n ret\n"
    "_store1: str s0,    [ptr]\n ret\n"

    "_loadX:  ldp q0,q4, [ptr]\n ret\n"
    "_loadX1: ldr s0,    [ptr]\n ret\n"

    "_loadY:  ldp q1,q5, [ptr]\n ret\n"
    "_loadY1: ldr s1,    [ptr]\n ret\n"

    "_add: fadd v0.4s,v0.4s,v1.4s\n"
    "      fadd v4.4s,v4.4s,v5.4s\n"
    "      ret                   \n"

    "_mul: fmul v0.4s,v0.4s,v1.4s\n"
    "      fmul v4.4s,v4.4s,v5.4s\n"
    "      ret                   \n"
);
#else

#endif
