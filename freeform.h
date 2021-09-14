#pragma once

void store(void); void store1(void);
void loadX(void); void loadX1(void);
void loadY(void); void loadY1(void);
void   add(void);
void   mul(void);
void  done(void);

void inc(void); void inc1(void);

void ptr1(void); void update1(void);
void ptr2(void); void update2(void);
void ptr3(void); void update3(void);
void ptr4(void); void update4(void);
void ptr5(void); void update5(void);
void ptr6(void); void update6(void);
void ptr7(void); void update7(void);

void args(int, void*, void*, void*, void*, void*, void*, void*);

void interp(void(*program[])(void));
