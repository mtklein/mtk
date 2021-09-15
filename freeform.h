#pragma once

void store_8(void); void store_1(void);
void loadX_8(void); void loadX_1(void);
void loadY_8(void); void loadY_1(void);
void   add(void);
void   mul(void);
void  done(void);

void ptrA(void); void incA(void);
void ptrB(void); void incB(void);
void ptrC(void); void incC(void);
void ptrD(void); void incD(void);
void ptrE(void); void incE(void);
void ptrF(void); void incF(void);

void interp(int n, void* A, void* B, void* C, void* D, void* E, void* F, void(*program[])(void));
