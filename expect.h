#pragma once

#include <stdio.h>
#include <stdlib.h>

#define expect(cond) \
    if (!(cond)) fprintf(stderr, "%s:%d expect(%s) failed\n", __FILE__, __LINE__, #cond), abort()
