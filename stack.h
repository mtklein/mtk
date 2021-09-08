#pragma once

#define N 4
typedef float __attribute__((ext_vector_type(N), aligned(sizeof(float)))) V;

typedef void (Op)(void*[], void* data[], int n, V,V,V,V,V,V,V,V);

Op load, uniform, store,
   add, mul,
   done;

void run(Op*[], void* data[], int n);
