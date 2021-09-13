#include "freeform.h"

#if defined(__aarch64__)
__asm__(
    ".align 4"                       "\n"

    "program .req x0"                "\n"
    "args    .req x1"                "\n"
    "i       .req x2"                "\n"

    "ptr     .req x9"                "\n"
    "fn      .req x10"               "\n"
    "stash   .req x11"               "\n"

    ".global _ptr"                   "\n"
    ".global _store"                 "\n"
    ".global _loadX"                 "\n"
    ".global _loadY"                 "\n"
    ".global _add"                   "\n"
    ".global _mul"                   "\n"
    ".global _interp"                "\n"
    ".global _done"                  "\n"

    "_ptr:"                          "\n"
        "ldr ptr,[args],8"           "\n"   // ptr = *args++
        "ret"                        "\n"

    "_store:"                        "\n"
        "add ptr,ptr,i,lsl 5"        "\n"   // ptr += 32*i
        "stp q0,q4,[ptr]"            "\n"
        "ret"                        "\n"

    "_loadX:"                        "\n"
        "add ptr,ptr,i,lsl 5"        "\n"
        "ldp q0,q4,[ptr]"            "\n"
        "ret"                        "\n"

    "_loadY:"                        "\n"
        "add ptr,ptr,i,lsl 5"        "\n"
        "ldp q1,q5,[ptr]"            "\n"
        "ret"                        "\n"

    "_add:"                          "\n"
        "fadd v0.4s,v0.4s,v1.4s"     "\n"
        "fadd v4.4s,v4.4s,v5.4s"     "\n"
        "ret"                        "\n"

    "_mul:"                          "\n"
        "fmul v0.4s,v0.4s,v1.4s"     "\n"
        "fmul v4.4s,v4.4s,v5.4s"     "\n"
        "ret"                        "\n"

    "_interp:"                       "\n"
        "mov stash,lr"               "\n"   // save lr
        "bl  loop"                   "\n"   // effectively, lr = loop
    "loop:"                          "\n"
        "ldr fn,[program],8"         "\n"   // fn = *program++
        "br  fn"                     "\n"   // fn(), returning to loop

    "_done:"                         "\n"
        "ret stash"                  "\n"   // actually return
);
#else

#endif
