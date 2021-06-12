#pragma once

extern "C" {
    int dprintf(int, const char*, ...);
    void abort(void);
}

#define expect(cond) \
    if (!(cond)) dprintf(2, "%s:%d expect(%s) failed\n", __FILE__, __LINE__, #cond), abort()
