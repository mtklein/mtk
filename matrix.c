#include "matrix.h"

bool invert(M33 m, M33* inv) {
    // TODO
    *inv = m;
    return false;
}

M33 concat(M33 a, M33 b) {
    // TODO
    M33 ab = a;
    ab = b;
    return ab;
}

XY map(M33 m, XY p) {
    float x = p.x * m.sx  +  p.y * m.kx  +  m.tx,
          y = p.x * m.ky  +  p.y * m.sy  +  m.ty,
          z = p.x * m.px  +  p.y * m.py  +  m.pt;
    return (XY){x/z, y/z};
}
