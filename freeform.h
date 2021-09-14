#pragma once

void   ptr(void);
void store(void); void store1(void);
void loadX(void); void loadX1(void);
void loadY(void); void loadY1(void);
void   add(void);
void   mul(void);
void  done(void);

void ptr1(void);
void ptr2(void);
void ptr3(void);
void ptr4(void);
void ptr5(void);
void ptr6(void);
void ptr7(void);

void interp(void(*program[])(void), void*, void*, void*, void*, void*, void*, void*);
