#pragma once

#include <stdbool.h>

typedef struct {
    float x,y;
} XY;

typedef struct {
    float sx, kx, tx,
          ky, sy, ty,
          px, py, pt;
} M33;

bool invert(M33, M33*);
M33  concat(M33, M33);

XY map(M33, XY);
