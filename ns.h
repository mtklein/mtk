#pragma once

extern struct ns {
    const int constantA,
              constantB;
    int (*add)(int, int);
    int (*mul)(int, int);
} ns;
