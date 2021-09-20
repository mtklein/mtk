#include "freeform.h"

#if defined(__aarch64__)
__asm__(
    ".align 4"                       "\n"

    "n       .req x0"                "\n"
    "A       .req x1"                "\n"
    "B       .req x2"                "\n"
    "C       .req x3"                "\n"
    "D       .req x4"                "\n"
    "E       .req x5"                "\n"
    "F       .req x6"                "\n"
    "program .req x7"                "\n"

    "ptr     .req x9"                "\n"
    "fn      .req x10"               "\n"
    "stash   .req x11"               "\n"
    "inst    .req x12"               "\n"

    ".global _interp"                "\n"
    ".global _done"                  "\n"

    ".global _ptrA"                  "\n"
    ".global _ptrB"                  "\n"
    ".global _ptrC"                  "\n"
    ".global _ptrD"                  "\n"
    ".global _ptrE"                  "\n"
    ".global _ptrF"                  "\n"

    ".global _incA"                  "\n"
    ".global _incB"                  "\n"
    ".global _incC"                  "\n"
    ".global _incD"                  "\n"
    ".global _incE"                  "\n"
    ".global _incF"                  "\n"

    ".global _store_8"               "\n"
    ".global _store_1"               "\n"
    ".global _loadX_8"               "\n"
    ".global _loadX_1"               "\n"
    ".global _loadY_8"               "\n"
    ".global _loadY_1"               "\n"

    ".global _add"                   "\n"
    ".global _mul"                   "\n"

    "_interp:"                       "\n"
        "mov stash,lr"               "\n"  // stash = lr      (preserve ultimate return address)
        "adr lr,2f"                  "\n"  // lr = inner loop (setup so br fn will return to 2:)

    "1:"                             "\n"  // top of outer loop over n:
        "mov inst,program"           "\n"  //   inst will step, program will stay put

    "2:"                             "\n"  // top of inner loop over instructions:
        "ldr fn,[inst],8"            "\n"  //   fn = *inst++
        "br  fn"                     "\n"  //   fn(), return to 2:, the top of this inner loop

    "_done:"                         "\n"  // always the last instruction,
        "subs n,n,1"                 "\n"  //   n -= 1
        "b.ne 1b"                    "\n"  //   if (!n) goto 1:, the top of the outer loop over n
        "ret  stash"                 "\n"  //   otherwise return to stashed return address

    "_ptrA: mov ptr,A"  "\n"  "ret"  "\n"
    "_ptrB: mov ptr,B"  "\n"  "ret"  "\n"
    "_ptrC: mov ptr,C"  "\n"  "ret"  "\n"
    "_ptrD: mov ptr,D"  "\n"  "ret"  "\n"
    "_ptrE: mov ptr,E"  "\n"  "ret"  "\n"
    "_ptrF: mov ptr,F"  "\n"  "ret"  "\n"

    "_incA: mov A,ptr"  "\n"  "ret"  "\n"
    "_incB: mov B,ptr"  "\n"  "ret"  "\n"
    "_incC: mov C,ptr"  "\n"  "ret"  "\n"
    "_incD: mov D,ptr"  "\n"  "ret"  "\n"
    "_incE: mov E,ptr"  "\n"  "ret"  "\n"
    "_incF: mov F,ptr"  "\n"  "ret"  "\n"

    "_store_8: stp q0,q4, [ptr],32"  "\n"  "ret"  "\n"
    "_store_1: str s0,    [ptr], 4"  "\n"  "ret"  "\n"

    "_loadX_8: ldp q0,q4, [ptr],32"  "\n"  "ret"  "\n"
    "_loadX_1: ldr s0,    [ptr], 4"  "\n"  "ret"  "\n"

    "_loadY_8: ldp q1,q5, [ptr],32"  "\n"  "ret"  "\n"
    "_loadY_1: ldr s1,    [ptr], 4"  "\n"  "ret"  "\n"

    "_add: fadd v0.4s,v0.4s,v1.4s"  "\n"
    "      fadd v4.4s,v4.4s,v5.4s"  "\n"
    "      ret                   "  "\n"

    "_mul: fmul v0.4s,v0.4s,v1.4s"  "\n"
    "      fmul v4.4s,v4.4s,v5.4s"  "\n"
    "      ret                   "  "\n"
);
#else

#endif
